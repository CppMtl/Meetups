#include "AsyncButton.h"
#include "AsyncSystem.hpp"
#include "catch.hpp"
#include <future>
#include <thread>

SCENARIO("A button can be clicked (event log comparison)", "[button]")
{
	GIVEN("An ordinary button") {
		std::vector<Button::Event> evenements;
		AsyncSystem<Button> b{[&evenements](Button::Event e) {
			evenements.push_back(e);
		}};
		
		WHEN("it is clicked") {
			b.Apply(MouseClick{});
			b.WaitForAllTasks();
			
			THEN("the clickCount is adjusted") {
				REQUIRE(evenements.size() == 3ul);
				REQUIRE(evenements[0].index() == Button::Event{OnMouseDown{}}.index());
				REQUIRE(evenements[1].index() == Button::Event{OnMouseUp{}}.index());
				REQUIRE(evenements[2].index() == Button::Event{OnMouseClick{}}.index());
				REQUIRE(std::get<OnMouseClick>(evenements[2]).clickCount == 1);
			}
		}
		WHEN("it is clicked twice very quickly") {
			b.Apply(MouseClick{});
			b.Apply(MouseClick{});
			b.WaitForAllTasks();
			
			THEN("the clickCount is incremented") {
				REQUIRE(evenements.size() == 6ul);
				REQUIRE(evenements[0].index() == Button::Event{OnMouseDown{}}.index());
				REQUIRE(evenements[1].index() == Button::Event{OnMouseUp{}}.index());
				REQUIRE(evenements[2].index() == Button::Event{OnMouseClick{}}.index());
				REQUIRE(std::get<OnMouseClick>(evenements[2]).clickCount == 1);
				REQUIRE(evenements[3].index() == Button::Event{OnMouseDown{}}.index());
				REQUIRE(evenements[4].index() == Button::Event{OnMouseUp{}}.index());
				REQUIRE(evenements[5].index() == Button::Event{OnMouseClick{}}.index());
				REQUIRE(std::get<OnMouseClick>(evenements[5]).clickCount == 2);
			}
		}
		WHEN("it is clicked again after long enough") {
			b.Apply(MouseClick{});
			std::this_thread::sleep_for(std::chrono::milliseconds{500});
			b.Apply(MouseClick{});
			b.WaitForAllTasks();
			
			THEN("the clickCount starts over") {
				REQUIRE(evenements.size() == 6ul);
				REQUIRE(evenements[0].index() == Button::Event{OnMouseDown{}}.index());
				REQUIRE(evenements[1].index() == Button::Event{OnMouseUp{}}.index());
				REQUIRE(evenements[2].index() == Button::Event{OnMouseClick{}}.index());
				REQUIRE(std::get<OnMouseClick>(evenements[2]).clickCount == 1);
				REQUIRE(evenements[3].index() == Button::Event{OnMouseDown{}}.index());
				REQUIRE(evenements[4].index() == Button::Event{OnMouseUp{}}.index());
				REQUIRE(evenements[5].index() == Button::Event{OnMouseClick{}}.index());
				REQUIRE(std::get<OnMouseClick>(evenements[5]).clickCount == 1);
			}
		}
		WHEN("it is pushed") {
			b.Apply(MouseDown{});
			b.WaitForAllTasks();
			
			THEN("it doesn't count as a click yet") {
				REQUIRE(evenements.size() == 1ul);
				REQUIRE(evenements[0].index() == Button::Event{OnMouseDown{}}.index());
			}
		}
		WHEN("it is pushed and released") {
			b.Apply(MouseDown{});
			b.Apply(MouseUp{});
			b.WaitForAllTasks();
			
			THEN("it is considered as being clicked") {
				REQUIRE(evenements.size() == 3ul);
				REQUIRE(evenements[0].index() == Button::Event{OnMouseDown{}}.index());
				REQUIRE(evenements[1].index() == Button::Event{OnMouseUp{}}.index());
				REQUIRE(evenements[2].index() == Button::Event{OnMouseClick{}}.index());
				REQUIRE(std::get<OnMouseClick>(evenements[2]).clickCount == 1);
			}
		}
		WHEN("it is pushed, mouse moves and it is released out of bounds") {
			b.Apply(MouseDown{});
			b.Apply(MouseMove{});
			b.Apply(MouseUp{});
			b.WaitForAllTasks();
			
			THEN("it is not considered as a click") {
				REQUIRE(evenements.size() == 3ul);
				REQUIRE(evenements[0].index() == Button::Event{OnMouseDown{}}.index());
				REQUIRE(evenements[1].index() == Button::Event{OnMouseMove{}}.index());
				REQUIRE(evenements[2].index() == Button::Event{OnMouseUp{}}.index());
			}
		}
	}
}
