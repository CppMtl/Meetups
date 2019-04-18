#ifndef AsyncSystem_hpp
#define AsyncSystem_hpp

#include <algorithm>
#include <deque>
#include <thread>
#include "VariableCondition.hpp"
#include <variant>
#include <vector>

template <typename T>
class AsyncSystem
{
public:
	using Action = typename T::Action;
	using Event = typename T::Event;
	using EventHandler = std::function<void(Event)>;
	
public:
	AsyncSystem()
	:	t{[](Event){}},
		thread{&AsyncSystem::SystemThread, this}
	{}
	
	AsyncSystem(EventHandler consumeFct)
	: t{consumeFct},
		thread{&AsyncSystem::SystemThread, this}
	{}
	
	~AsyncSystem()
	{
		InterruptAndWait();
	}
	
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
		{
			auto tasks = asyncThreadTasks.Verrouiller();
			tasks->closeInitiated = true;
		}
		if (thread.joinable()) {
			asyncThreadTasks.ReveillerTous();
			thread.join();
		}
	}
	
private:
	void SystemThread()
	{
		bool keepGoing{true};
		do {
			std::function<void()> block;
			{
				auto tasks = asyncThreadTasks.Verrouiller();
				auto& taskQueue = tasks->taskQueue;
//				asyncSystem->donneesFilCorrecteur->enFermeture = tasks->enFermeture;
				if (!taskQueue.empty()) {
					block = taskQueue.front();
					taskQueue.pop_front();
				} else if (tasks->closeInitiated) {
					keepGoing = false;
				} else {
					tasks.Attendre();
				}
			}
			if (block) {
				block();
			}
		} while (keepGoing);
	}
	
	void AddTask(std::function<void()> task)
	{
		bool etaitVide = false;
		{
			auto tasks = asyncThreadTasks.Verrouiller();
			auto& taskQueue = tasks->taskQueue;
			etaitVide = taskQueue.empty();
			taskQueue.push_back(task);
		}
		if (etaitVide) {
			asyncThreadTasks.ReveillerUn();
		}
	}
	
private:
	struct Tasks {
		bool closeInitiated{false};
		std::deque<std::function<void()>> taskQueue;
	};
	mutable VariableCondition<Tasks> asyncThreadTasks;
	T t;
	std::thread thread;
};

#endif /* AsyncSystem_hpp */
