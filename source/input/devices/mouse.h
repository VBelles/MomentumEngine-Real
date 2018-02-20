#pragma once

#include "input/device.h"

namespace Input
{
	class CMouse : public IDevice
	{
	public:
		CMouse(const std::string& name);
		virtual void updateMouseData(float delta, TInterface_Mouse& data) override;

		void setButton(EMouseButton bt, bool pressed);
		void setPosition(float posX, float posY);
		void setDelta(float dX, float dY);
		void setWheelDelta(float delta);

	private:
		bool _buttons[MOUSE_BUTTONS];
		VEC2 _position;
		VEC2 positionDelta;
		float _wheel_delta;
	};
}