#include <cassert>
#include <chrono>
#include <iostream>
#include <mutex>
#include <string>
#include <thread_pool>

namespace execution = std::execution;
using std::static_thread_pool;

struct strand_state
{
  std::mutex mutex_;
  std::list<std::function<void()>> queue_;
  bool locked_{false};
  std::thread::id owning_thread_;
};

template <class Executor, class Blocking = execution::blocking_t::possibly_t>
class strand
{
  template <class, class> friend class strand;

  std::shared_ptr<strand_state> state_;
  Executor ex_;

  template <class T> static auto inner_declval() -> decltype(std::declval<Executor>());

  strand(std::shared_ptr<strand_state> s, Executor ex)
    : state_(std::move(s)), ex_(std::move(ex))
  {
  }

  void run_first_item()
  {
    // Dequeue first item.
    std::unique_lock<std::mutex> lock(state_->mutex_);
    std::function<void()> f = std::move(state_->queue_.front());
    state_->queue_.pop_front();
    state_->owning_thread_ = std::this_thread::get_id();
    lock.unlock();

    // Execute the item.
    std::cout << "begin strand\n";
    f();
    std::cout << "end strand\n";

    // Check if there are any more items.
    lock.lock();
    state_->owning_thread_ = std::thread::id{};
    if (state_->queue_.empty())
    {
      state_->locked_ = false;
      return;
    }
    lock.unlock();

    // Need to reschedule the strand to run the queued items.
    Executor ex(ex_);
    ex.execute([s = std::move(*this)]() mutable
        {
          s.run_first_item();
        });
  }

public:
#if defined(__cpp_concepts)
  static_assert(execution::OneWayExecutor<Executor>, "strand requires a one way executor");
#else
  static_assert(execution::is_oneway_executor_v<Executor>, "strand requires a one way executor");
#endif

  explicit strand(Executor ex)
    : state_(std::make_shared<strand_state>()), ex_(std::move(ex))
  {
  }

  static constexpr auto query(execution::executor_concept_t) { return execution::oneway; }

  template <class Property> auto require(const Property& p) const
    -> strand<decltype(inner_declval<Property>().require(p)), Blocking>
      { return { state_, ex_.require(p) }; }

  auto require(execution::blocking_t::never_t) const
    -> strand<decltype(std::declval<Executor>().require(execution::blocking.never)), execution::blocking_t::never_t>
  {
    return {state_, ex_.require(execution::blocking.never)};
  };

  auto require(execution::blocking_t::possibly_t) const
    -> strand<decltype(std::declval<Executor>().require(execution::blocking.possibly)), execution::blocking_t::possibly_t>
  {
    return {state_, ex_.require(execution::blocking.possibly)};
  };

  void require(execution::blocking_t::always_t) const = delete;

  template<class Property> auto query(const Property& p) const
    -> decltype(inner_declval<Property>().query(p))
      { return ex_.query(p); }

  friend bool operator==(const strand& a, const strand& b) noexcept
  {
    return a.state_ == b.state_;
  }

  friend bool operator!=(const strand& a, const strand& b) noexcept
  {
    return a.state_ != b.state_;
  }

  template <class Function>
  void execute(Function f) const
  {
    std::unique_lock<std::mutex> lock(state_->mutex_);

    // Determine if we are already inside the strand.
    bool in_strand = (state_->owning_thread_ == std::this_thread::get_id());

    // Execute immediately if possible.
    if (std::is_same<Blocking, execution::blocking_t::possibly_t>::value && in_strand)
    {
      lock.unlock();
      f();
      return;
    }

    // Try to grab the strand "lock".
    state_->queue_.emplace_back(std::move(f));
    if (state_->locked_) return; // Someone else already holds the "lock".
    state_->locked_ = true;
    lock.unlock();

    // Need to schedule the strand to run the queued items.
    ex_.execute([s = this->require(execution::blocking.never).require(execution::relationship.continuation)]() mutable
        {
          s.run_first_item();
        });
  }
};

#if defined(__cpp_concepts)
static_assert(execution::OneWayExecutor<
  strand<static_thread_pool::executor_type>>,
    "one way executor concept not satisfied");
#else
static_assert(execution::is_oneway_executor_v<
  strand<static_thread_pool::executor_type>>,
    "one way executor requirements must be met");
#endif

struct foo
{
  decltype(std::declval<strand<static_thread_pool::executor_type>>().require(execution::blocking.never)) strand_;
  int count_{0};

  explicit foo(const strand<static_thread_pool::executor_type>& s)
    : strand_(s.require(execution::blocking.never))
  {
  }

  void operator()()
  {
    if (count_ < 10)
    {
      std::this_thread::sleep_for(std::chrono::seconds(1));
      std::cout << "count is " << count_ << "\n";
      strand_.require(execution::blocking.possibly).execute([count = count_]{ std::cout << "nested count is " << count << "\n"; });
      ++count_;
      strand_.execute(*this);
    }
  }
};

int main()
{
  static_thread_pool pool{2};
  strand<static_thread_pool::executor_type> s1(pool.executor());
  assert(&std::query(s1, execution::context) == &pool);
  std::require(s1, execution::blocking.never).execute(foo{s1});
  std::require(s1, execution::blocking.possibly).execute([]{ std::cout << "After 0, before 1\n"; });
  pool.wait();
}
