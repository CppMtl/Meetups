#include <experimental/thread_pool>
#include <iostream>

namespace execution = std::experimental::execution;
using std::experimental::static_thread_pool;

template <class Executor, class Function>
auto invoke(Executor ex, Function f)
{
  return execution::prefer(execution::require(ex, execution::twoway), execution::blocking.always).twoway_execute(std::move(f)).get();
}

int main()
{
  static_thread_pool pool{1};
  int result = invoke(pool.executor(), []{ return 42; });
  std::cout << "result is " << result << "\n";
}
