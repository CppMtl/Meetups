#ifndef ASYNC_BUTTON_H
#define ASYNC_BUTTON_H

#include <chrono>
#include <variant>

struct MouseClick {};
struct MouseDown {};
struct MouseMove {};
struct MouseUp {};

struct OnMouseClick {
	unsigned int clickCount;
};
struct OnMouseDown {};
struct OnMouseMove {
	bool pressInitiated;
	bool pressed;
};
struct OnMouseUp {};

class Button
{
public:
	using Action = std::variant<MouseClick, MouseDown, MouseMove, MouseUp>;
	using Event = std::variant<OnMouseClick, OnMouseDown, OnMouseMove, OnMouseUp>;
	
	using EventHandler = std::function<void(Event)>;
	
public:
	Button();
	Button(EventHandler);
	
	unsigned int getClickCount() const;
	bool isPressed() const;
	
	void setEventHandler(EventHandler);
	
	void mouseClick();
	void mouseDown();
	void mouseMove();
	void mouseUp();
	
	void Apply(MouseClick) {mouseClick();}
	void Apply(MouseDown) {mouseDown();}
	void Apply(MouseMove) {mouseMove();}
	void Apply(MouseUp) {mouseUp();}
	
private:
	void fireEvent(Event);
	void handleClick();
	
private:
	bool pressInitiated = false;
	bool pressed = false;
	unsigned int clickCount = 0;
	std::chrono::steady_clock::time_point lastClickTime;
	EventHandler delegate;
};

#endif
