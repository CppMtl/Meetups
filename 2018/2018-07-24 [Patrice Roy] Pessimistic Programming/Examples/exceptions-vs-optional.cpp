#include <chrono>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <utility>
#include <optional>
using namespace std;
using namespace std::chrono;
class Darn {};
int f(int n) {
   if (n > 0) return -n;
   throw Darn {};
}
optional<int> g(int n) {
   if (n > 0) return -n;
   return {};
}
auto generate_nice_data(int n) {
   vector<int> v(n);
   iota(begin(v), end(v), 1);
   return v;
}
auto generate_nasty_data(int n) {
   auto v = generate_nice_data(n);
   transform(begin(v), end(v), begin(v), [](auto && n) { return -n; });
   return v;
}
template <class F, class ... Args>
   auto test(F f, Args &&... args) {
      auto pre = high_resolution_clock::now();
      auto res = f(std::forward<Args>(args)...);
      auto post = high_resolution_clock::now();
      return pair(res, post - pre);
   }
int main() {
   enum { N = 1'000'000 };
   auto r0 = test([v = generate_nice_data(N)] {
      int succeed = 0, fail = 0;
      for(auto n : v)
         try {
            f(n);
            ++succeed;
         } catch(Darn&) {
            ++fail;
         }
      return pair(succeed, fail);
   });
   auto r1 = test([v = generate_nasty_data(N)] {
      int succeed = 0, fail = 0;
      for(auto n : v)
         try {
            f(n);
            ++succeed;
         } catch(Darn&) {
            ++fail;
         }
      return pair(succeed, fail);
   });
   auto r2 = test([v = generate_nice_data(N)] {
      int succeed = 0, fail = 0;
      for(auto n : v)
         if (g(n))
            ++succeed;
         else
            ++fail;
      return pair(succeed, fail);
   });
   auto r3 = test([v = generate_nasty_data(N)] {
      int succeed = 0, fail = 0;
      for(auto n : v)
         if (g(n))
            ++succeed;
         else
            ++fail;
      return pair(succeed, fail);
   });
   cout << "With exceptions, nice data  : (" << r0.first.first << "," << r0.first.second << ") in " << duration_cast<microseconds>(r0.second).count() << "us." << endl;
   cout << "With exceptions, nasty data : (" << r1.first.first << "," << r1.first.second << ") in " << duration_cast<microseconds>(r1.second).count() << "us." << endl;
   cout << "With optional, nice data    : (" << r2.first.first << "," << r2.first.second << ") in " << duration_cast<microseconds>(r2.second).count() << "us." << endl;
   cout << "With optional, nasty data   : (" << r3.first.first << "," << r3.first.second << ") in " << duration_cast<microseconds>(r3.second).count() << "us." << endl;
}
