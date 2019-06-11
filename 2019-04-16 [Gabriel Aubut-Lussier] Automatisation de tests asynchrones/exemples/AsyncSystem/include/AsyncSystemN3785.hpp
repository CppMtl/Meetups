#ifndef AsyncSystemN3785_hpp
#define AsyncSystemN3785_hpp

#define BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION
#define BOOST_THREAD_PROVIDES_EXECUTORS
#define BOOST_THREAD_USES_MOVE
#include <boost/thread/executors/basic_thread_pool.hpp>
#include <boost/thread/executors/serial_executor.hpp>
#include "VariableCondition.hpp"
#include <variant>

template <typename T>
class AsyncSystemN3785
{
public:
	using Action = typename T::Action;
	using Event = typename T::Event;
	using EventHandler = std::function<void(Event)>;
	
public:
	AsyncSystemN3785()
	:	t{[](Event){}}
	{}
	
	AsyncSystemN3785(EventHandler consumeFct)
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
		VariableCondition<bool> derniereTache;
		auto verrou = derniereTache.Verrouiller();
		
		AddTask([&]() {
			auto verrou = derniereTache.Verrouiller();
			derniereTache.ReveillerTous();
		});
		
		verrou.Attendre();
	}
	
	void InterruptAndWait()
	{
		executor.close();
		threadPool.close();
		threadPool.interrupt_and_join();
	}
	
private:
	template <typename Closure>
	void AddTask(Closure&& task)
	{
		executor.submit(std::forward<Closure>(task));
	}
	
private:
	T t;
	boost::basic_thread_pool threadPool{1};
	boost::serial_executor executor{threadPool};
};

#endif /* AsyncSystem_hpp */
