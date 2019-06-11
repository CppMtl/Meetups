#include <execution>
#include <iostream>

namespace execution = std::execution;

class inline_executor
{
public:
  static constexpr auto query(execution::executor_concept_t) { return execution::bulk_oneway; }

  friend bool operator==(const inline_executor&, const inline_executor&) noexcept
  {
    return true;
  }

  friend bool operator!=(const inline_executor&, const inline_executor&) noexcept
  {
    return false;
  }

  template <class Function, class SharedFactory>
  void bulk_execute(Function f, std::size_t n, SharedFactory sf) const noexcept
  {
    auto shared_state(sf());
    for (std::size_t i = 0; i < n; ++i)
      f(i, shared_state);
  }
};

#if defined(__cpp_concepts)
static_assert(execution::BulkOneWayExecutor<inline_executor>, "bulk one way executor concept not satisfied");
static_assert(!execution::OneWayExecutor<inline_executor>, "one way executor concept incorrectly satisfied");
#else
static_assert(execution::is_bulk_oneway_executor_v<inline_executor>, "bulk one way executor requirements not met");
static_assert(!execution::is_oneway_executor_v<inline_executor>, "must not meet one way executor requirements");
#endif

int main()
{
  inline_executor ex1;
  ex1.bulk_execute([](int n, int&){ std::cout << "part " << n << "\n"; }, 8, []{ return 0; });
}
