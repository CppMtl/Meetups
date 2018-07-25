#include <experimental/generator>
#include <iostream>
#include <chrono>
#include <vector>
using namespace std;
using namespace std::chrono;
using experimental::generator;

template <class Pred>
generator<vector<int>> even_integers(Pred pred) {
   int n = 0;
   vector<int> res;
   for (;;) {
      if (!pred()) {
         co_yield res;
         res.clear();
      }
      res.emplace_back(n);
      n += 2;
   }
}
int main() {
   auto deadline = system_clock::now() + 500us;
   for (auto n : even_integers([&deadline] { return system_clock::now() < deadline; })) {
      cout << "\nComputed " << n.size() << " even integers" << endl;
      for (auto m : n) cout << m << ' ';
      cout << endl << endl;
      deadline = system_clock::now() + 1ms;
   }
}