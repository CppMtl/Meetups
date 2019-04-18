#ifndef BUTTON_CONTROLLED_MISSILE_SYSTEM_H
#define BUTTON_CONTROLLED_MISSILE_SYSTEM_H

#include "AsyncButton.h"
#include "AsyncSystem.hpp"

class ButtonControlledMissileSystem
{
public:
	ButtonControlledMissileSystem();
	
	bool areMissilesArmed() const;
	unsigned int getMissilesFired() const;
	
	template <typename T>
	void Apply(T t)
	{
		b.Apply(t);
	}
	
	void Apply(Button::Action a)
	{
		b.Apply(a);
	}
	
	void WaitForAllTasks()
	{
		b.WaitForAllTasks();
	}
	
private:
	Button::EventHandler getEventHandler();
	
private:
	AsyncSystem<Button> b;
	bool missilesArmed = false;
	unsigned int missilesFired = 0;
};

#endif
