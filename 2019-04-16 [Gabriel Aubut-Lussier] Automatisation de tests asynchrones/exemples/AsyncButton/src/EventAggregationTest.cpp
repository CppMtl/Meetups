#include "AsyncButton.h"
#include "AsyncSystem.hpp"
#include "ButtonControlledMissileSystem.h"
#include "catch.hpp"
#include <thread>

SCENARIO("Missiles can be launched with a simple button", "[ButtonControlledMissileSystem]")
{
	GIVEN("A missile system") {
		ButtonControlledMissileSystem b;
		
		WHEN("its button is clicked") {
			b.Apply(MouseClick{});
			b.WaitForAllTasks();
			
			THEN("a missile is fired") {
				REQUIRE(b.getMissilesFired() == 1);
			}
		}
		WHEN("it is clicked twice very quickly") {
			b.Apply(MouseClick{});
			b.Apply(MouseClick{});
			b.WaitForAllTasks();
			
			THEN("a single missile is fired, because people do get anxious about launching those things") {
				REQUIRE(b.getMissilesFired() == 1);
			}
		}
		WHEN("it is clicked again after long enough") {
			b.Apply(MouseClick{});
			std::this_thread::sleep_for(std::chrono::milliseconds{500});
			b.Apply(MouseClick{});
			b.WaitForAllTasks();
			
			THEN("two missiles are launched") {
				REQUIRE(b.getMissilesFired() == 2);
			}
		}
		WHEN("it is pushed") {
			b.Apply(MouseDown{});
			b.WaitForAllTasks();
			
			THEN("nothing is launched, but missiles are armed") {
				REQUIRE(b.getMissilesFired() == 0);
				REQUIRE(b.areMissilesArmed());
			}
		}
		WHEN("it is pushed and released") {
			b.Apply(MouseDown{});
			b.Apply(MouseUp{});
			b.WaitForAllTasks();
			
			THEN("a missile is fired") {
				REQUIRE(b.getMissilesFired() == 1);
			}
		}
		WHEN("it is pushed, mouse moves and it is released out of bounds") {
			b.Apply(MouseDown{});
			b.Apply(MouseMove{});
			b.Apply(MouseUp{});
			b.WaitForAllTasks();
			
			THEN("missiles are not launched, disaster is avoided") {
				REQUIRE(b.getMissilesFired() == 0);
				REQUIRE(!b.areMissilesArmed());
			}
		}
	}
}
