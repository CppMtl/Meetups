#include "AsyncButton.h"
#include "catch.hpp"
#include <thread>

SCENARIO("A button can be clicked (synchronous)", "[button]")
{
	GIVEN("An ordinary button") {
		Button b;
		
		REQUIRE(!b.isPressed());
		REQUIRE(b.getClickCount() == 0);
		
		WHEN("it is clicked") {
			b.mouseClick();
			
			THEN("the clickCount is adjusted") {
				REQUIRE(b.getClickCount() == 1);
			}
		}
		WHEN("it is clicked twice very quickly") {
			b.mouseClick();
			b.mouseClick();
			
			THEN("the clickCount is incremented") {
				REQUIRE(b.getClickCount() == 2);
			}
		}
		WHEN("it is clicked again after long enough") {
			b.mouseClick();
			std::this_thread::sleep_for(std::chrono::milliseconds{500});
			b.mouseClick();
			
			THEN("the clickCount starts over") {
				REQUIRE(b.getClickCount() == 1);
			}
		}
		WHEN("it is pushed") {
			b.mouseDown();
			
			THEN("it doesn't count as a click yet") {
				REQUIRE(b.getClickCount() == 0);
			}
		}
		WHEN("it is pushed and released") {
			b.mouseDown();
			b.mouseUp();
			
			THEN("it is considered as being clicked") {
				REQUIRE(b.getClickCount() == 1);
			}
		}
		WHEN("it is pushed, mouse moves and it is released out of bounds") {
			b.mouseDown();
			b.mouseMove();
			b.mouseUp();
			
			THEN("it is not considered as a click") {
				REQUIRE(b.getClickCount() == 0);
			}
		}
	}
}
