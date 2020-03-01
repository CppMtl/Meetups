#ifndef VariableCondition_h
#define VariableCondition_h

#include <condition_variable>
#include <mutex>

template <typename TypeVariable>
class VariableCondition
{
public:
	class Verrou
	{
	public:
		explicit Verrou(VariableCondition* parent, std::unique_lock<std::mutex>&& verrou) : parent(parent), verrou(std::move(verrou)) {}
		Verrou(const Verrou&) = delete;
		Verrou(Verrou&&) = default;
		
		TypeVariable* operator->() {return std::addressof(parent->variable);}
		TypeVariable& operator*() {return parent->variable;}
		
		void Attendre()
		{
			parent->condition.wait(verrou);
		}
		
		template <class Rep, class Period>
		std::cv_status AttendreDuree(const std::chrono::duration<Rep, Period>& duree)
		{
			return parent->condition.wait_for(verrou, duree);
		}
		
		template <class Rep, class Period, class Predicate>
		std::cv_status AttendreDuree(const std::chrono::duration<Rep, Period>& duree, Predicate pred)
		{
			return parent->condition.wait_for(verrou, duree, std::forward<Predicate>(pred));
		}
		
		template <class Clock, class Duration>
		std::cv_status AttendreJusquA(const std::chrono::time_point<Clock, Duration>& seuilAttente)
		{
			return parent->condition.wait_until(verrou, seuilAttente);
		}
		
		template <class Clock, class Duration, class Predicate>
		bool AttendreJusquA(const std::chrono::time_point<Clock, Duration>& seuilAttente, Predicate pred)
		{
			return parent->condition.wait_until(verrou, seuilAttente, std::forward<Predicate>(pred));
		}
		
	private:
		VariableCondition* parent;
		std::unique_lock<std::mutex> verrou;
	};
	
	Verrou Verrouiller()
	{
		return Verrou(this, std::unique_lock<std::mutex>{m});
	}
	
	void ReveillerUn() noexcept
	{
		condition.notify_one();
	}
	
	void ReveillerTous() noexcept
	{
		condition.notify_all();
	}
	
	friend Verrou;
	
private:
	TypeVariable variable;
	std::mutex m;
	std::condition_variable condition;
};

#endif /* VariableCondition_h */
