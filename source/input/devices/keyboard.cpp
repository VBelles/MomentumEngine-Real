#include "mcv_platform.h"
#include "keyboard.h"
#include "input/device.h"
#include "windows/app.h"

namespace Input {
	void CKeyboard::updateKeyboardData(float delta, TInterface_Keyboard& data) {
		bool app_has_focus = CApp::get().isWindowFocused();
		for (int i = 0; i < Input::NUM_KEYBOARD_KEYS; ++i) {
			bool ok = isPressed(i) && app_has_focus;
			data._keys[i].update(delta, ok ? 1.f : 0.f);
		}
	}
}

