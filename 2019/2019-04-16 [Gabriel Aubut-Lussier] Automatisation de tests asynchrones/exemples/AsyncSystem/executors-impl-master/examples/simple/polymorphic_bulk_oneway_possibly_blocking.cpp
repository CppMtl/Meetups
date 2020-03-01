#include <thread_pool>
#include <iostream>

namespace execution = std::execution;
using std::static_thread_pool;

using executor = execution::executor<
  execution::bulk_oneway_t>;

int main()
{
  static_thread_pool pool{1};
  executor ex = std::require(
      std::require_concept(pool.executor(), execution::bulk_oneway),
      execution::blocking.possibly);
  ex.bulk_execute([](int n, int&){ std::cout << "part " << n << "\n"; }, 8, []{ return 0; });
  pool.wait();
}
