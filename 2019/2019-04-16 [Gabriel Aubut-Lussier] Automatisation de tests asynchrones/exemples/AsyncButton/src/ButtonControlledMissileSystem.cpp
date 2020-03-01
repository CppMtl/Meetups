#include "ButtonControlledMissileSystem.h"

#include "AsyncButton.h"

ButtonControlledMissileSystem::ButtonControlledMissileSystem()
: b{getEventHandler()}
{}

bool
ButtonControlledMissileSystem::areMissilesArmed() const
{
	return missilesArmed;
}

unsigned int
ButtonControlledMissileSystem::getMissilesFired() const
{
	return missilesFired;
}

Button::EventHandler
ButtonControlledMissileSystem::getEventHandler()
{
	return [this](Button::Event e) {
		std::visit([this](auto&& event) mutable {
			using T = std::decay_t<decltype(event)>;
			if constexpr (std::is_same_v<T, OnMouseDown>) {
				missilesArmed = true;
			} else if constexpr (std::is_same_v<T, OnMouseMove>) {
				missilesArmed = !missilesArmed;
			} else if constexpr (std::is_same_v<T, OnMouseUp>) {
				// nothing to do here
			} else if constexpr (std::is_same_v<T, OnMouseClick>) {
				if (missilesArmed && event.clickCount == 1) {
					++missilesFired;
				}
			} else {
				static_assert(!std::is_same_v<T, T>, "non-exhaustive visitor!");
			}
		}, e);
	};
}
