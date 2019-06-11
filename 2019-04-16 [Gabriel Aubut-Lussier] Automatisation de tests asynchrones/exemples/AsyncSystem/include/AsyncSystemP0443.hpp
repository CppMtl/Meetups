#ifndef AsyncSystemP0443_hpp
#define AsyncSystemP0443_hpp

#include "thread_pool"
#include "VariableCondition.hpp"
#include <variant>

template <typename T>
class AsyncSystemP0443
{
public:
	using Action = typename T::Action;
	using Event = typename T::Event;
	using EventHandler = std::function<void(Event)>;
	
public:
	AsyncSystemP0443()
	:	t{[](Event){}}
	{}
	
	AsyncSystemP0443(EventHandler consumeFct)
	: t{consumeFct}
	{}
	
	void Apply(Action a)
	{
		std::visit([this](auto&& a) {
			AddTask([this, a]() {
				t.Apply(a);
			});
		}, a);
	}
	
	void WaitForAllTasks()
	{
		threadPool.wait();
	}
	
	void InterruptAndWait()
	{
		threadPool.stop();
		threadPool.wait();
	}
	
private:
	template <typename Closure>
	void AddTask(Closure&& task)
	{
		threadPool.executor().execute(std::forward<Closure>(task));
	}
	
private:
	T t;
	std::static_thread_pool threadPool{1};
};

#endif /* AsyncSystem_hpp */
