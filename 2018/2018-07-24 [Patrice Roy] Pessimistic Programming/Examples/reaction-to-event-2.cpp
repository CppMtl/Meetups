#include <optional>
#include <mutex>
#include <vector>
#include <functional>
#include <memory>
#include <thread>
#include <iostream>
#include <deque>
#include <chrono>
using namespace std;
using namespace std::chrono;

class Event {};

optional<Event> next_event() {
   if (char c; cin.get(c)) return { {} };
   return {};
}
void reaction_to_event(Event) {
   cout << "An event!" << endl;
}
int main() {
   vector<function<void(Event)>> to_call;
   // ...
   // ... the thread-safe requirement on
   // called-back functions goes away
   to_call.emplace_back(reaction_to_event);
   to_call.emplace_back([](Event e) { cout << "Wow!" << endl; });
   deque<Event> to_process;
   decltype(to_call.size()) pos{};
   static const auto iter_time = 10ms;
   for (auto cur = system_clock::now(); ; cur = system_clock::now()) {
      // ... consumption phase
      if (auto e = next_event(); e) { // could be a while instead of an if
         to_process.push_back(e.value());
      }
      // ... processing phase
      while (/*enough_time_current_iteration()*/system_clock::now() < cur + iter_time && !to_process.empty()) {
         if (pos != to_call.size()) {
            to_call[pos](to_process.front());
            ++pos;
         } else {
            to_process.pop_front();
            pos = {};
         }
      }
      // wait_for_next_cycle();
      this_thread::sleep_until(cur + iter_time);
   }

}
