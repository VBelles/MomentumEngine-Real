#include "mcv_platform.h"
#include "keyboard.h"
#include "input/device.h"
#include <string>

namespace Input {
	void CKeyboard::updateKeyboardData(float delta, TInterface_Keyboard& data) {
		for (int i = 0; i < Input::NUM_KEYBOARD_KEYS; ++i) {
			bool ok = isPressed(i);
			data._keys[i].update(delta, ok ? 1.f : 0.f);
		}
	}
}