#include <random>
#include <chrono>
#include <thread>
#include <iostream>
#include <fstream>
#include <optional>
using namespace std;
using namespace std::chrono;
class Data {};
istream& operator>>(istream &is, Data&) {
   if (char c; is) is.get(c);
   return is;
}
// almost same construct, slight rewrite
// switching to non-blocking I/O
optional<Data> try_consume(istream &is) { // non-blocking
   // I'm cheating here, to keep things simple
   if (Data data; is >> data) return { data };
   return {};
}
void process(const Data&) {
   static mt19937 prng{ random_device{}() };
   this_thread::sleep_for(milliseconds{ uniform_int_distribution<int>{1, 10}(prng) });
   cout << '.' << flush;
}
void accessory_tasks() {
   static const auto init = system_clock::now();
   const auto dt = system_clock::now() - init;
   cout << "accessory tasks performed at "
        << duration_cast<milliseconds>(dt).count() << " ms." << endl;
}
class ConsumeError {};
[[noreturn]] void processing_loop(istream &source) {
   for (;;) {
      if (auto data = try_consume(source); data) {
         process(data.value());
      }
      else if (!source) {
         throw ConsumeError{};
      }
      accessory_tasks();
   }
}
int main() try {
   ifstream in{ "in.txt" };
   processing_loop(in);
} catch(ConsumeError &) {
   cerr << "Error consuming data from stream\n";
}