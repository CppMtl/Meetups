#include "AsyncButton.h"
#include <cassert>

Button::Button()
: delegate([](Event){})
{}

Button::Button(EventHandler eh)
: delegate(eh)
{}

unsigned int
Button::getClickCount() const
{
	return clickCount;
}

bool
Button::isPressed() const
{
	return pressed;
}

void
Button::setEventHandler(EventHandler eh)
{
	delegate = eh;
}

void
Button::mouseClick()
{
	assert(!pressInitiated);
	assert(!pressed);
	fireEvent(OnMouseDown{});
	fireEvent(OnMouseUp{});
	handleClick();
	fireEvent(OnMouseClick{clickCount});
}

void
Button::mouseDown()
{
	assert(!pressInitiated);
	assert(!pressed);
	pressInitiated = true;
	pressed = true;
	fireEvent(OnMouseDown{});
}

void
Button::mouseMove()
{
	if (pressInitiated) {
		pressed = !pressed;
		fireEvent(OnMouseMove{pressInitiated, pressed});
	}
}

void
Button::mouseUp()
{
	fireEvent(OnMouseUp{});
	pressInitiated = false;
	if (pressed) {
		pressed = false;
		handleClick();
		fireEvent(OnMouseClick{clickCount});
	}
}

void
Button::fireEvent(Event e)
{
	delegate(e);
}

void
Button::handleClick()
{
	auto now = std::chrono::steady_clock::now();
	auto elapsedTime = now - lastClickTime;
	if (elapsedTime > std::chrono::milliseconds{400}) {
		clickCount = 1;
	} else {
		++clickCount;
	}
	lastClickTime = now;
}
