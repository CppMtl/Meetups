#include <random>
#include <chrono>
#include <thread>
#include <iostream>
#include <fstream>
using namespace std;
using namespace std::chrono;
class Data {};
istream& operator>>(istream &is, Data&) {
   if (char c; is) is.get(c);
   return is;
}
void process(const Data&) {
   static mt19937 prng{ random_device{}() };
   this_thread::sleep_for(milliseconds{ uniform_int_distribution<int>{1, 10}(prng) });
   cout << '.' << flush;
}
class ConsumeError {};
[[noreturn]] void processing_loop(istream &source) {
   for (;;) {
      // expected to fail only in extreme cases
      if (Data data; source >> data) {
         process(data);
      }
      else {
         // expected to be extremely rare
         throw ConsumeError{};
      }
   }
}
int main() try {
   ifstream in{ "in.txt" };
   processing_loop(in);
} catch(ConsumeError &) {
   cerr << "Error consuming data from stream\n";
}