#pragma once

#include "mcv_platform.h"
#include "input/button.h"

namespace Input {
	struct TInterface_Mouse {
		TButton buttons[MOUSE_BUTTONS];
		VEC2 position;
		VEC2 position_delta;
		float wheel_delta;

		const TButton& button(EMouseButton bt) const {
			return buttons[bt];
		}
	};
}
