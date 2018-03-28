#include "mcv_platform.h"
#include "input/devices/mouse.h"

namespace Input {
	CMouse::CMouse(const std::string& name)
		: IDevice(name) {
		_buttons[MOUSE_LEFT] = false;
		_buttons[MOUSE_MIDDLE] = false;
		_buttons[MOUSE_RIGHT] = false;
		_wheel_delta = 0.f;
		_position = VEC2(0.5f, 0.5f);
	}

	void CMouse::updateMouseData(float delta, TInterface_Mouse& data) {
		// buttons
		data.buttons[MOUSE_LEFT].update(delta, _buttons[MOUSE_LEFT] ? 1.f : 0.f);
		data.buttons[MOUSE_MIDDLE].update(delta, _buttons[MOUSE_MIDDLE] ? 1.f : 0.f);
		data.buttons[MOUSE_RIGHT].update(delta, _buttons[MOUSE_RIGHT] ? 1.f : 0.f);

		// position
		data.position_delta = positionDelta;
		positionDelta = VEC2::Zero;
		data.position = _position;

		// wheel
		data.wheel_delta = _wheel_delta;
		_wheel_delta = 0.f;
	}

	void CMouse::setButton(EMouseButton bt, bool pressed) {
		_buttons[bt] = pressed;
	}

	void CMouse::setPosition(float posX, float posY) {
		_position = VEC2(posX, posY);
	}

	void CMouse::setDelta(float dX, float dY) {
		positionDelta = VEC2(dX, dY);
	}

	void CMouse::setWheelDelta(float delta) {
		_wheel_delta += delta;
	}

}
