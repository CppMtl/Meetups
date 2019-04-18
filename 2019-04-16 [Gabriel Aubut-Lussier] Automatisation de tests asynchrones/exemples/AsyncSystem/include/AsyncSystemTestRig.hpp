#ifndef AsyncSystemTestRig_hpp
#define AsyncSystemTestRig_hpp

#include "AsyncSystem.hpp"
#include <functional>
#include <optional>
#include "VariableCondition.hpp"
#include <vector>

template <typename T>
class AsyncSystemTestRig : public AsyncSystem<T>
{
public:
	using Action = typename T::Action;
	using Event = typename T::Event;
	
public:
	AsyncSystemTestRig()
	: AsyncSystem<T>{std::bind(&AsyncSystemTestRig::Consume, this, std::placeholders::_1)}
	{}
	
	~AsyncSystemTestRig()
	{
		this->InterruptAndWait();
	}
	
	template <typename E>
	std::optional<E> Get(const std::chrono::seconds& tempsAttenteMax = std::chrono::seconds{2})
	{
		std::optional<E> result;
		auto threshold = std::chrono::steady_clock::now() + tempsAttenteMax;
		auto queue = eventQueue.Verrouiller();
		auto& elements = *queue;
		auto predicate = [&elements]() -> bool {
			return !elements.empty();
		};
		do {
			if (predicate()) {
				auto& front = elements.front();
				if (std::holds_alternative<E>(front)) {
					result = std::move(std::get<E>(front));
				}
				elements.erase(elements.begin());
			}
		} while (!result && std::chrono::steady_clock::now() < threshold && queue.AttendreJusquA(threshold, predicate));
		return result;
	}
	
private:
	void Consume(Event e)
	{
		bool processEvents{false};
		{
			auto journal = eventQueue.Verrouiller();
			bool wasEmpty = journal->empty();
			journal->push_back(e);
			processEvents = wasEmpty && !journal->empty();
		}
		if (processEvents) {
			eventQueue.ReveillerUn();
		}
	}
	
private:
	VariableCondition<std::vector<Event>> eventQueue;
};

#endif
