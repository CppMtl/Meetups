#include <chrono>
#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <numeric>
#include <utility>
#include <optional>
using namespace std;
using namespace std::chrono;
template <class F, class ... Args>
auto test(F f, Args &&... args) {
   auto pre = high_resolution_clock::now();
   auto res = f(std::forward<Args>(args)...);
   auto post = high_resolution_clock::now();
   return pair(res, post - pre);
}
int main() {
   enum { N = 1'000'000 };
   auto r0 = test([] {
      vector<int> v;
      v.reserve(N);
      for (int i = 0; i != N; ++i)
         if (i % 2 == 0)
            v.push_back(i);
      return v.back();
   });
   auto r1 = test([] {
      deque<int> v;
      // v.reserve(N);
      for (int i = 0; i != N; ++i)
         if (i % 2 == 0)
            v.push_back(i);
      return v.back();
   });
   cout << "With vector : " << duration_cast<microseconds>(r0.second).count() << "us." << endl;
   cout << "With deque  : " << duration_cast<microseconds>(r1.second).count() << "us." << endl;
}
