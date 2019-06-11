#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <thread_pool>

namespace execution = std::execution;
using std::static_thread_pool;

template <class InnerExecutor>
class logging_executor
{
  std::shared_ptr<std::string> prefix_;
  InnerExecutor inner_ex_;

  template <class T> static auto inner_declval() -> decltype(std::declval<InnerExecutor>());

  template <class Function>
  auto wrap(Function f) const
  {
    return [prefix = *prefix_, f = std::move(f)]() mutable
        {
          std::cout << prefix << ": " << "function begins\n";

          struct on_exit
          {
            std::string& prefix;
            ~on_exit() { std::cout << prefix << ": " << "function ends\n"; }
          } x{prefix};

          return f();
        };
  }

public:
  logging_executor(const std::string& prefix, const InnerExecutor& ex)
    : prefix_(std::make_shared<std::string>(prefix)), inner_ex_(ex) {}

  template <class Property> auto require(const Property& p) const &
    -> logging_executor<decltype(inner_declval<Property>().require(p))>
      { return { *prefix_, inner_ex_.require(p) }; }
  template <class Property> auto require(const Property& p) &&
    -> logging_executor<decltype(inner_declval<Property>().require(p))>
      { return { *prefix_, std::move(inner_ex_).require(p) }; }

  static constexpr auto query(execution::executor_concept_t) { return execution::oneway; }

  template<class Property> auto query(const Property& p) const
    -> decltype(inner_declval<Property>().query(p))
      { return inner_ex_.query(p); }

  friend bool operator==(const logging_executor& a, const logging_executor& b) noexcept
  {
    return *a.prefix_ == *b.prefix_ && a.inner_ex_ == b.inner_ex_;
  }

  friend bool operator!=(const logging_executor& a, const logging_executor& b) noexcept
  {
    return !(a == b);
  }

  template <class Function>
  auto execute(Function f) const
    -> decltype(inner_declval<Function>().execute(std::move(f)))
  {
    return inner_ex_.execute(this->wrap(std::move(f)));
  }
};

#if defined(__cpp_concepts)
static_assert(execution::OneWayExecutor<
  logging_executor<static_thread_pool::executor_type>>,
    "one way executor concept not satisfied");
#else
static_assert(execution::is_oneway_executor_v<
  logging_executor<static_thread_pool::executor_type>>,
    "one way executor requirements must be met");
#endif

int main()
{
  static_thread_pool pool{1};
  logging_executor<static_thread_pool::executor_type> ex1("LOG", pool.executor());
  assert(&std::query(ex1, execution::context) == &pool);
  ex1.execute([]{ std::cout << "we made it\n"; });
  auto ex2 = std::require(ex1, execution::blocking.always);
  ex2.execute([]{ std::cout << "we made it again\n"; });
  auto ex3 = std::require(ex2, execution::blocking.never, execution::relationship.continuation);
  ex3.execute([]{ std::cout << "and again\n"; });
  pool.wait();
}
