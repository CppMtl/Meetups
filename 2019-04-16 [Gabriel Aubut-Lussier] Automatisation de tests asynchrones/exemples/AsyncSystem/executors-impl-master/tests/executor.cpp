#include <execution>
#include <thread_pool>

namespace execution = std::execution;
using std::static_thread_pool;

using oneway_executor = execution::executor<
    execution::oneway_t,
    execution::bulk_oneway_t,
    execution::mapping_t::thread_t,
    execution::blocking_t::never_t,
    execution::blocking_t::possibly_t,
    execution::blocking_t::always_t
  >;

using bulk_oneway_executor = execution::executor<
    execution::bulk_oneway_t,
    execution::oneway_t,
    execution::mapping_t::thread_t,
    execution::blocking_t::never_t,
    execution::blocking_t::possibly_t,
    execution::blocking_t::always_t
  >;

void oneway_executor_compile_test()
{
#if defined(__cpp_concepts)
  static_assert(execution::OneWayExecutor<oneway_executor>, "must satisfy OneWayExecutor concept");
#else
  static_assert(execution::is_oneway_executor_v<oneway_executor>, "is_oneway_executor must evaluate true");
#endif

  static_thread_pool pool(0);

  static_assert(noexcept(oneway_executor()), "default constructor must not throw");
  static_assert(noexcept(oneway_executor(nullptr)), "nullptr constructor must not throw");

  oneway_executor ex1;
  oneway_executor ex2(nullptr);
  ex2.require_concept(execution::oneway);

  const oneway_executor& cex1 = ex1;
  const oneway_executor& cex2 = ex2;

  static_assert(noexcept(oneway_executor(cex1)), "copy constructor must not throw");
  static_assert(noexcept(oneway_executor(std::move(ex1))), "move constructor must not throw");

  oneway_executor ex3(ex1);
  oneway_executor ex4(std::move(ex1));

  oneway_executor ex5(pool.executor());

  execution::executor<execution::oneway_t> ex6(ex5);
  execution::executor<execution::oneway_t, execution::bulk_oneway_t> ex7(ex5);

  static_assert(noexcept(ex2 = cex1), "copy assignment must not throw");
  static_assert(noexcept(ex3 = std::move(ex1)), "move assignment must not throw");
  static_assert(noexcept(ex3 = nullptr), "nullptr assignment must not throw");

  ex2 = ex1;
  ex3 = std::move(ex1);
  ex4 = nullptr;
  ex5 = pool.executor();

  static_assert(noexcept(ex1.swap(ex2)), "swap must not throw");

  ex1.swap(ex2);

  ex1.assign(pool.executor());

  bulk_oneway_executor ex8 = std::require_concept(cex1, execution::bulk_oneway);

  ex1 = std::require_concept(cex1, execution::oneway);
  ex1 = std::require(cex1, execution::mapping.thread);
  ex1 = std::require(cex1, execution::blocking.never);
  ex1 = std::require(cex1, execution::blocking.possibly);
  ex1 = std::require(cex1, execution::blocking.always);

  ex1 = std::prefer(cex1, execution::mapping.thread);
  ex1 = std::prefer(cex1, execution::blocking.never);
  ex1 = std::prefer(cex1, execution::blocking.possibly);
  ex1 = std::prefer(cex1, execution::blocking.always);
  ex1 = std::prefer(cex1, execution::relationship.fork);
  ex1 = std::prefer(cex1, execution::relationship.continuation);
  ex1 = std::prefer(cex1, execution::outstanding_work.untracked);
  ex1 = std::prefer(cex1, execution::outstanding_work.tracked);
  ex1 = std::prefer(cex1, execution::bulk_guarantee.sequenced);
  ex1 = std::prefer(cex1, execution::bulk_guarantee.parallel);
  ex1 = std::prefer(cex1, execution::bulk_guarantee.unsequenced);
  ex1 = std::prefer(cex1, execution::mapping.new_thread);

  cex1.execute([]{});

  bool b1 = static_cast<bool>(ex1);
  (void)b1;

  const std::type_info& target_type = cex1.target_type();
  (void)target_type;

  static_thread_pool::executor_type* ex9 = ex1.target<static_thread_pool::executor_type>();
  (void)ex9;

  const static_thread_pool::executor_type* cex6 = ex1.target<static_thread_pool::executor_type>();
  (void)cex6;

  bool b2 = (cex1 == cex2);
  (void)b2;

  bool b3 = (cex1 != cex2);
  (void)b3;

  bool b4 = (cex1 == nullptr);
  (void)b4;

  bool b5 = (cex1 != nullptr);
  (void)b5;

  bool b6 = (nullptr == cex2);
  (void)b6;

  bool b7 = (nullptr != cex2);
  (void)b7;

  swap(ex1, ex2);
}

void bulk_oneway_executor_compile_test()
{
#if defined(__cpp_concepts)
  static_assert(execution::BulkOneWayExecutor<bulk_oneway_executor>, "must satisfy BulkOneWayExecutor concept");
#else
  static_assert(execution::is_bulk_oneway_executor_v<bulk_oneway_executor>, "is_bulk_oneway_executor must evaluate true");
#endif

  static_thread_pool pool(0);

  static_assert(noexcept(bulk_oneway_executor()), "default constructor must not throw");
  static_assert(noexcept(bulk_oneway_executor(nullptr)), "nullptr constructor must not throw");

  bulk_oneway_executor ex1;
  bulk_oneway_executor ex2(nullptr);

  const bulk_oneway_executor& cex1 = ex1;
  const bulk_oneway_executor& cex2 = ex2;

  static_assert(noexcept(bulk_oneway_executor(cex1)), "copy constructor must not throw");
  static_assert(noexcept(bulk_oneway_executor(std::move(ex1))), "move constructor must not throw");

  bulk_oneway_executor ex3(ex1);
  bulk_oneway_executor ex4(std::move(ex1));

  bulk_oneway_executor ex5(pool.executor());

  execution::executor<execution::oneway_t> ex6(ex5);
  execution::executor<execution::oneway_t, execution::bulk_oneway_t> ex7(ex5);

  static_assert(noexcept(ex2 = cex1), "copy assignment must not throw");
  static_assert(noexcept(ex3 = std::move(ex1)), "move assignment must not throw");
  static_assert(noexcept(ex3 = nullptr), "nullptr assignment must not throw");

  ex2 = ex1;
  ex3 = std::move(ex1);
  ex4 = nullptr;
  ex5 = pool.executor();

  static_assert(noexcept(ex1.swap(ex2)), "swap must not throw");

  ex1.swap(ex2);

  ex1.assign(pool.executor());

  oneway_executor ex8 = std::require_concept(cex1, execution::oneway);

  ex1 = std::require_concept(cex1, execution::bulk_oneway);
  ex1 = std::require(cex1, execution::mapping.thread);
  ex1 = std::require(cex1, execution::blocking.never);
  ex1 = std::require(cex1, execution::blocking.possibly);
  ex1 = std::require(cex1, execution::blocking.always);

  ex1 = std::prefer(cex1, execution::mapping.thread);
  ex1 = std::prefer(cex1, execution::blocking.never);
  ex1 = std::prefer(cex1, execution::blocking.possibly);
  ex1 = std::prefer(cex1, execution::blocking.always);
  ex1 = std::prefer(cex1, execution::relationship.fork);
  ex1 = std::prefer(cex1, execution::relationship.continuation);
  ex1 = std::prefer(cex1, execution::outstanding_work.untracked);
  ex1 = std::prefer(cex1, execution::outstanding_work.tracked);
  ex1 = std::prefer(cex1, execution::bulk_guarantee.sequenced);
  ex1 = std::prefer(cex1, execution::bulk_guarantee.parallel);
  ex1 = std::prefer(cex1, execution::bulk_guarantee.unsequenced);
  ex1 = std::prefer(cex1, execution::mapping.new_thread);

  cex1.bulk_execute([](std::size_t, int&){}, 1, []{ return 42; });

  bool b1 = static_cast<bool>(ex1);
  (void)b1;

  const std::type_info& target_type = cex1.target_type();
  (void)target_type;

  static_thread_pool::executor_type* ex9 = ex1.target<static_thread_pool::executor_type>();
  (void)ex9;

  const static_thread_pool::executor_type* cex6 = ex1.target<static_thread_pool::executor_type>();
  (void)cex6;

  bool b2 = (cex1 == cex2);
  (void)b2;

  bool b3 = (cex1 != cex2);
  (void)b3;

  bool b4 = (cex1 == nullptr);
  (void)b4;

  bool b5 = (cex1 != nullptr);
  (void)b5;

  bool b6 = (nullptr == cex2);
  (void)b6;

  bool b7 = (nullptr != cex2);
  (void)b7;

  swap(ex1, ex2);
}

template <class Interface>
class inline_executor
{
public:
  friend bool operator==(const inline_executor&, const inline_executor&) noexcept
  {
    return true;
  }

  friend bool operator!=(const inline_executor&, const inline_executor&) noexcept
  {
    return false;
  }

  inline_executor<execution::oneway_t> require(execution::oneway_t) const noexcept
  {
    return {};
  }

  template <class Function>
  auto execute(Function f) const noexcept
    -> std::enable_if_t<
      std::is_same_v<Function, Function> && std::is_same_v<Interface, execution::oneway_t>,
      void>
  {
    f();
  }
};

int main()
{
}
