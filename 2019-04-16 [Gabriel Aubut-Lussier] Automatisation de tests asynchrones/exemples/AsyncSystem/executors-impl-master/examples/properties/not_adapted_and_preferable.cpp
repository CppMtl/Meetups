#include <thread_pool>
#include <cassert>
#include <iostream>

namespace execution = std::execution;
using std::static_thread_pool;

namespace custom_props
{
  struct tracing
  {
    static constexpr bool is_requirable_concept = false;
    static constexpr bool is_requirable = true;
    static constexpr bool is_preferable = true;
    using polymorphic_query_result_type = bool;

    bool on = false;
  };
};

namespace std
{
#if defined(__cpp_concepts)
  template<execution::Executor E>
  struct is_applicable_property<E, ::custom_props::tracing>
    : std::true_type {};
#else
  template<class Entity>
  struct is_applicable_property<Entity, ::custom_props::tracing,
    std::enable_if_t<execution::is_executor_v<Entity>>>
      : std::true_type {};
#endif
}

class inline_executor
{
public:
  static constexpr auto query(execution::executor_concept_t) { return execution::oneway; }

  inline_executor require(custom_props::tracing t) const { inline_executor tmp(*this); tmp.tracing_ = t.on; return tmp; }

  bool query(custom_props::tracing) const { return tracing_; }

  friend bool operator==(const inline_executor&, const inline_executor&) noexcept
  {
    return true;
  }

  friend bool operator!=(const inline_executor&, const inline_executor&) noexcept
  {
    return false;
  }

  template <class Function>
  void execute(Function f) const noexcept
  {
    if (tracing_) std::cout << "running function inline\n";
    f();
  }

private:
  bool tracing_;
};

#if defined(__cpp_concepts)
static_assert(execution::OneWayExecutor<inline_executor>, "one way executor concept not satisfied");
#else
static_assert(execution::is_oneway_executor_v<inline_executor>, "one way executor requirements not met");
#endif

int main()
{
  static_thread_pool pool{1};

  auto ex1 = std::require(inline_executor(), custom_props::tracing{true});
  assert(std::query(ex1, custom_props::tracing{}));
  ex1.execute([]{ std::cout << "we made it\n"; });

  auto ex2 = std::prefer(inline_executor(), custom_props::tracing{true});
  assert(std::query(ex2, custom_props::tracing{}));
  ex2.execute([]{ std::cout << "we made it with a preference\n"; });

  // No adaptation means we can't require arbitrary executors using our custom property ...
  static_assert(!std::can_require_v<static_thread_pool::executor_type, custom_props::tracing>, "can't require tracing from static_thread_pool");
  static_assert(!std::can_query_v<static_thread_pool::executor_type, custom_props::tracing>, "can't query tracing from static_thread_pool");

  // ... but we can still ask for it as a preference.
  auto ex3 = std::prefer(pool.executor(), custom_props::tracing{true});
  static_assert(!std::can_query_v<decltype(ex3), custom_props::tracing>, "cannot query tracing for static_thread_pool::executor");
  ex3.execute([]{ std::cout << "we made it again with a preference\n"; });
  pool.wait();
}
