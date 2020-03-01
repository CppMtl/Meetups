#include "AsyncButton.h"
#include "AsyncSystemTestRig.hpp"
#include "AsyncSystemN3785.hpp"
#include "catch.hpp"
#include <thread>

SCENARIO("A button can be clicked (event consumption with N3785)", "[button]")
{
	GIVEN("An ordinary button") {
		AsyncSystemTestRig<Button, AsyncSystemN3785> b;
		
		WHEN("it is clicked") {
			b.Apply(MouseClick{});
			
			THEN("the clickCount is adjusted") {
				auto onClickOpt = b.Get<OnMouseClick>();
				REQUIRE(onClickOpt);
				REQUIRE(onClickOpt->clickCount == 1);
			}
		}
		WHEN("it is clicked twice very quickly") {
			b.Apply(MouseClick{});
			b.Apply(MouseClick{});
			
			THEN("the clickCount is incremented") {
				{
					auto onClickOpt = b.Get<OnMouseClick>();
					REQUIRE(onClickOpt);
					REQUIRE(onClickOpt->clickCount == 1);
				}
				{
					auto onClickOpt = b.Get<OnMouseClick>();
					REQUIRE(onClickOpt);
					REQUIRE(onClickOpt->clickCount == 2);
				}
			}
		}
		WHEN("it is clicked again after long enough") {
			b.Apply(MouseClick{});
			std::this_thread::sleep_for(std::chrono::milliseconds{500});
			b.Apply(MouseClick{});
			
			THEN("the clickCount starts over") {
				{
					auto onClickOpt = b.Get<OnMouseClick>();
					REQUIRE(onClickOpt);
					REQUIRE(onClickOpt->clickCount == 1);
				}
				{
					auto onClickOpt = b.Get<OnMouseClick>();
					REQUIRE(onClickOpt);
					REQUIRE(onClickOpt->clickCount == 1);
				}
			}
		}
		WHEN("it is pushed") {
			b.Apply(MouseDown{});
			
			THEN("it doesn't count as a click yet") {
				REQUIRE(b.Get<OnMouseDown>());
				REQUIRE(false); // Comment prouver qu'il n'y a pas de click?
			}
		}
		WHEN("it is pushed and released") {
			b.Apply(MouseDown{});
			b.Apply(MouseUp{});
			
			THEN("it is considered as being clicked") {
				auto onClickOpt = b.Get<OnMouseClick>();
				REQUIRE(onClickOpt);
				REQUIRE(onClickOpt->clickCount == 1);
			}
		}
		WHEN("it is pushed, mouse moves and it is released out of bounds") {
			b.Apply(MouseDown{});
			b.Apply(MouseMove{});
			b.Apply(MouseUp{});
			
			THEN("it is not considered as a click") {
				REQUIRE(b.Get<OnMouseUp>());
				REQUIRE(false); // Comment prouver qu'il n'y a pas de click?
			}
		}
	}
}
