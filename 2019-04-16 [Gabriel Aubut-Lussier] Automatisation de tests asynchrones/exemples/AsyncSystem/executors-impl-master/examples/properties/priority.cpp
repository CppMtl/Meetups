#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <execution>

namespace execution = std::execution;

namespace custom_props {

  struct priority
  {
    static constexpr bool is_requirable_concept = false;
    static constexpr bool is_requirable = true;
    static constexpr bool is_preferable = true;
    using polymorphic_query_result_type = int;

    int value() const { return value_; }

    int value_ = 1;
  };

  constexpr priority low_priority{0};
  constexpr priority normal_priority{1};
  constexpr priority high_priority{2};

} // namespace custom_props

namespace std
{
#if defined(__cpp_concepts)
  template<execution::Executor E>
  struct is_applicable_property<E, ::custom_props::priority>
    : std::true_type {};
#else
  template<class Entity>
  struct is_applicable_property<Entity, ::custom_props::priority,
    std::enable_if_t<execution::is_executor_v<Entity>>>
      : std::true_type {};
#endif
}

class priority_scheduler
{
public:
  // A class that satisfies the Executor requirements.
  class executor_type
  {
  public:
    executor_type(priority_scheduler& ctx) noexcept
      : context_(ctx), priority_(custom_props::normal_priority.value())
    {
    }

    static constexpr auto query(execution::executor_concept_t)
    {
      return execution::oneway;
    }

    priority_scheduler& query(execution::context_t) const noexcept
    {
      return context_;
    }

    int query(custom_props::priority) const noexcept
    {
      return priority_;
    }

    executor_type require(custom_props::priority pri) const
    {
      executor_type new_ex(*this);
      new_ex.priority_ = pri.value();
      return new_ex;
    }

    template <class Func>
    void execute(Func f) const
    {
      auto p(std::make_shared<item<Func>>(priority_, std::move(f)));
      std::lock_guard<std::mutex> lock(context_.mutex_);
      context_.queue_.push(p);
      context_.condition_.notify_one();
    }

    friend bool operator==(const executor_type& a,
        const executor_type& b) noexcept
    {
      return &a.context_ == &b.context_;
    }

    friend bool operator!=(const executor_type& a,
        const executor_type& b) noexcept
    {
      return &a.context_ != &b.context_;
    }

  private:
    priority_scheduler& context_;
    int priority_;
  };

  executor_type executor() noexcept
  {
    return executor_type(*const_cast<priority_scheduler*>(this));
  }

  void run()
  {
    std::unique_lock<std::mutex> lock(mutex_);
    for (;;)
    {
      condition_.wait(lock, [&]{ return stopped_ || !queue_.empty(); });
      if (stopped_)
        return;
      auto p(queue_.top());
      queue_.pop();
      lock.unlock();
      p->execute_(p);
      lock.lock();
    }
  }

  void stop()
  {
    std::lock_guard<std::mutex> lock(mutex_);
    stopped_ = true;
    condition_.notify_all();
  }

private:
  struct item_base
  {
    int priority_;
    void (*execute_)(std::shared_ptr<item_base>&);
  };

  template <class Func>
  struct item : item_base
  {
    item(int pri, Func f) : function_(std::move(f))
    {
      priority_ = pri;
      execute_ = [](std::shared_ptr<item_base>& p)
      {
        Func tmp(std::move(static_cast<item*>(p.get())->function_));
        p.reset();
        tmp();
      };
    }

    Func function_;
  };

  struct item_comp
  {
    bool operator()(
        const std::shared_ptr<item_base>& a,
        const std::shared_ptr<item_base>& b)
    {
      return a->priority_ < b->priority_;
    }
  };

  std::mutex mutex_;
  std::condition_variable condition_;
  std::priority_queue<
    std::shared_ptr<item_base>,
    std::vector<std::shared_ptr<item_base>>,
    item_comp> queue_;
  bool stopped_ = false;
};

int main()
{
  priority_scheduler sched;
  auto ex = std::require_concept(sched.executor(), execution::oneway);
  auto prefer_low = std::prefer(ex, custom_props::low_priority);
  auto low = std::require(ex, custom_props::low_priority);
  auto med = std::require(ex, custom_props::normal_priority);
  auto high = std::require(ex, custom_props::high_priority);
  execution::executor<execution::oneway_t, custom_props::priority> poly_high(high);
  prefer_low.execute([]{ std::cout << "1\n"; });
  low.execute([]{ std::cout << "11\n"; });
  low.execute([]{ std::cout << "111\n"; });
  med.execute([]{ std::cout << "2\n"; });
  med.execute([]{ std::cout << "22\n"; });
  high.execute([]{ std::cout << "3\n"; });
  high.execute([]{ std::cout << "33\n"; });
  high.execute([]{ std::cout << "333\n"; });
  poly_high.execute([]{ std::cout << "3333\n"; });
  std::require(ex, custom_props::priority{-1}).execute([&]{ sched.stop(); });
  sched.run();
  std::cout << "polymorphic query result = " << std::query(poly_high, custom_props::priority{}) << "\n";
}
