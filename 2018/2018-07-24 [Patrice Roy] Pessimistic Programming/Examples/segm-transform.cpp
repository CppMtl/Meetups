#include <utility>
#include <chrono>
#include <vector>
#include <thread>
#include <iostream>
using namespace std;
using namespace std::chrono;
// subdivided (segmented) version
template<class I, class O, class F, class Pred>
auto segm_transform(I bi, I ei, O bo, F f, Pred pred) {
   for (; bi != ei && pred(); ++bi, (void) ++bo)
      *bo = f(*bi);
   return pair{ bi, bo };
}
class Data {};
class ProcessedData {};
ProcessedData f(Data) {
   this_thread::sleep_for(50us);
   cout << '.';
   return {};
}
vector<Data> gather_data() {
   return vector<Data>{ 10, Data{} };
}
int main() {
   // ...
   auto now = [] { return system_clock::now(); };
   auto make_pred = [now] {
      return[now, deadline = now() + 2ms]{
         return now() < deadline;
      };
   };
   vector<Data> in = gather_data();
   vector<ProcessedData> out;
   // ...
   auto p = segm_transform(
      begin(in), end(in), back_inserter(out), f, make_pred()
   );
   for (; p.first != end(in);
          p = segm_transform(p.first, end(in), back_inserter(out), f, make_pred()))
      cout << endl;
}
