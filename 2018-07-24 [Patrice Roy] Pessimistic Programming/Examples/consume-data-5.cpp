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
   auto sleep = milliseconds{ uniform_int_distribution<int>{1, 10}(prng) };
   this_thread::sleep_for(sleep);
   clog << '(' << sleep.count() << ")\t" << flush;
}
void accessory_tasks(high_resolution_clock::time_point init, high_resolution_clock::time_point until) {
   auto pre = high_resolution_clock::now();
   this_thread::sleep_until(until);
   clog << "accessory tasks performed for "
        << duration_cast<milliseconds>(until - pre).count() << " ms, from "
        << duration_cast<milliseconds>(pre - init).count() << " to "
        << duration_cast<milliseconds>(until - init).count() << " ms." << endl;
}

class ConsumeError {};
[[noreturn]] void processing_loop(istream &source) {
   static const auto iter_time = 20ms;
   static const auto init = high_resolution_clock::now();
   for (auto cur = init; ; cur = high_resolution_clock::now()) {
      if (auto data = try_consume(source); data) {
         process(data.value());
      } else if (!source) {
         throw ConsumeError{};
      }
      accessory_tasks(init, cur + iter_time);
   }
}
int main() try {
   ofstream out{ "out.txt" };
   clog.rdbuf(out.rdbuf());
   ifstream in{ "in.txt" };
   processing_loop(in);
} catch(ConsumeError &) {
   cerr << "Error consuming data from stream\n";
}