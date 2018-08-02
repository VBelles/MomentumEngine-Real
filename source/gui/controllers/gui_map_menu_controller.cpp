#include "mcv_platform.h"
#include "gui_map_menu_controller.h"
#include "gui/widgets/gui_map_marker.h"
#include "gui/widgets/gui_button.h"
#include "modules/system_modules/scripting/scripting_player.h"

namespace GUI {
	void CMapMenuController::update(float delta) {
		auto& mouse = EngineInput[Input::PLAYER_1].mouse();
		if (mouse.position_delta.Length() > 0) {
			int i = 0;
			while (i < _options.size()) {
				CMapMarker* option = _options[i];
				if (option->overlaps(mouse.position)) {
					if (i != _currentOption) {
						setCurrentOption(i);
					}
					break;
				}
				i++;
			}
			if (i == _options.size()) {
				setCurrentOption(_currentOption);
			}
		}
		else {
			VEC2 leftAnalogInput = VEC2(
				EngineInput[Input::EPadButton::PAD_LANALOG_X].value,
				EngineInput[Input::EPadButton::PAD_LANALOG_Y].value);

			if (stickInputTimer.elapsed() > stickInputCooldown && leftAnalogInput.Length() > PAD_DEAD_ZONE) {
				stickInputTimer.reset();
				if (leftAnalogInput.y < -0.5) {
					setCurrentOption(_currentOption + 1);
				}
				else if (leftAnalogInput.y > 0.5) {
					setCurrentOption(_currentOption - 1);
				}
				//else if (leftAnalogInput.x < -0.5) {
				//	_options[_currentOption]->setCurrentOption(_options[_currentOption]->getCurrentOption() - 1);
				//}
				//else if (leftAnalogInput.x > 0.5) {
				//	_options[_currentOption]->setCurrentOption(_options[_currentOption]->getCurrentOption() + 1);
				//}
			}
			else {
				if (EngineInput["menu_down"].getsPressed()) {
					setCurrentOption(_currentOption + 1);
				}
				else if (EngineInput["menu_up"].getsPressed()) {
					setCurrentOption(_currentOption - 1);
				}
				//else if (EngineInput["menu_left"].getsPressed()) {
				//	_options[_currentOption]->setCurrentOption(_options[_currentOption]->getCurrentOption() - 1);
				//}
				//else if (EngineInput["menu_right"].getsPressed()) {
				//	_options[_currentOption]->setCurrentOption(_options[_currentOption]->getCurrentOption() + 1);
				//}
			}
		}

		if (_currentOption >= 0 && EngineInput["mouse_accept"].getsPressed()
			&& _options[_currentOption]->overlaps(mouse.position)) {
			_options[_currentOption]->setCurrentState(CButton::EState::ST_Pressed);
		}
		else if (_currentOption >= 0 && EngineInput["mouse_accept"].getsReleased()
			&& _options[_currentOption]->overlaps(mouse.position)) {
			VEC3 pos = _options[_currentOption]->getPos();
			ScriptingPlayer::teleportPlayer(pos.x, pos.y, pos.z);
		}

		if (EngineInput["menu_back"].getsReleased()) {
			EngineModules.changeGameState("main_menu");
		}
	}

	void CMapMenuController::registerOption(const std::string& name) {
		CWidget* wdgt = Engine.getGUI().getWidget(name, true);
		CMapMarker* marker = dynamic_cast<CMapMarker*>(wdgt);
		if (marker) {
			_options.push_back(marker);
		}
	}

	void CMapMenuController::setCurrentOption(int newOption) {
		for (auto& option : _options) {
			option->setCurrentState(CButton::EState::ST_Idle);
		}

		int nOptions = static_cast<int>(_options.size());
		_currentOption = (newOption + nOptions) % nOptions;

		_options[_currentOption]->setCurrentState(CButton::EState::ST_Selected);
	}
}
