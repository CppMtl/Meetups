#include <optional>
#include <mutex>
#include <vector>
#include <functional>
#include <memory>
#include <thread>
#include <iostream>
using namespace std;

class Event {};

optional<Event> next_event() {
   if (char c; cin.get(c)) return { {} };
   return {};
}
class Registry {
   mutex m;
   vector<function<void(Event)>> to_call;
public:
   template <class F> void subscribe(F f) {
      lock_guard _{ m };
      to_call.emplace_back(f);
   }
   void callback(Event e) {
      lock_guard _{ m };
      for (auto & f : to_call) f(e);
   }
   void execute() {
      for (;;)
         if (auto e = next_event(); e)
            callback(e.value());
   }
   // ...
};

void reaction_to_event(Event) {
   cout << "An event!" << endl;
}
int main() {
   auto reg = make_shared<Registry>();
   // functions passed to reg's
   // subscribe() function have to
   // be thread-safe when called back
   reg->subscribe(reaction_to_event);
   reg->subscribe([](Event e) { cout << "Wow!" << endl; });
   thread th{ [reg] { reg->execute(); } };
   // th.detach();
   th.join();
}
