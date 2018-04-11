#pragma once

#include "input/button.h"

namespace Input {
	struct TInterface_Pad {
		TButton _buttons[PAD_BUTTONS];
		bool connected = false;

		const TButton& button(EPadButton bt) const {
			return _buttons[bt];
		}
	};
}
