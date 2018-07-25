#include <iostream>
#include <chrono>
#include <random>
#include <thread>
using namespace std;
using namespace std::chrono;
// called on occasion, to check if time remains
bool time_slot_exceeded(system_clock::time_point base) {
   static const auto time_slice = 10ms;
   return system_clock::now() < base + time_slice;
}

void work() {
   static mt19937 prng{ random_device{}() };
   this_thread::sleep_for(milliseconds{ uniform_int_distribution<int>{1, 3}(prng) });
}

// old-school style (don't do this on today's computers)
enum class State { Init, StepA, StepB, StepC, Done };
void long_computation(State &state) {
   auto base = system_clock::now();
   switch (state) {
   case State::Init:
      cout << "Beginning Init" << endl;
      // ...
      work();
      if (time_slot_exceeded(base)) return;
      // ...
      cout << "Ending Init" << endl;
      state = State::StepA;
   case State::StepA: // [[fallthrough]]
      cout << "Beginning StepA" << endl;
      // ...
      work();
      if (time_slot_exceeded(base)) return;
      // ...
      cout << "Ending StepA" << endl;
      state = State::StepB;
   case State::StepB: // [[fallthrough]]
      cout << "Beginning StepB" << endl;
      // ...
      work();
      if (time_slot_exceeded(base)) return;
      // ...
      cout << "Ending StepB" << endl;
      state = State::StepC;
   case State::StepC: // [[fallthrough]]
      cout << "Beginning StepC" << endl;
      // ...
      work();
      if (time_slot_exceeded(base)) return;
      // ...
      cout << "Ending StepC" << endl;
      state = State::Done;
   }
}
int main() {
   for(State state = State::Init; state != State::Done; )
      long_computation(state);
}