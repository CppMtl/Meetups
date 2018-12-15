#include "ThreadGuard.hpp"

struct Obj
{
	void Test()
	{}
};

int main()
{
	ThreadGuard<Obj> corr;
	corr->Test();
	std::thread t{
		[&corr]() {
			corr->Test();
		}
	};
	t.join();
}
