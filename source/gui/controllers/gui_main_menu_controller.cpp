#include "mcv_platform.h"
#include "gui_main_menu_controller.h"
#include "gui/widgets/gui_button.h"

namespace GUI {
	void CMainMenuController::update(float delta) {
		VEC2 leftAnalogInput = VEC2(
			EngineInput[Input::EPadButton::PAD_LANALOG_X].value,
			EngineInput[Input::EPadButton::PAD_LANALOG_Y].value);
		if (leftAnalogInput.Length() > PAD_DEAD_ZONE) {
			if (leftAnalogInput.y < -0.5) {
				setCurrentOption(_currentOption + 1);
			}
			if (leftAnalogInput.y > 0.5) {
				setCurrentOption(_currentOption - 1);
			}
		}
		else {
			if (EngineInput["menu_down"].getsPressed()) {
				setCurrentOption(_currentOption + 1);
			}
			if (EngineInput["menu_up"].getsPressed()) {
				setCurrentOption(_currentOption - 1);
			}
		}

		if (EngineInput["menu_accept"].getsPressed()) {
			_options[_currentOption].button->setCurrentState(CButton::EState::ST_Pressed);
		}
		if (EngineInput["menu_accept"].getsReleased()) {
			_options[_currentOption].button->setCurrentState(CButton::EState::ST_Selected);
			_options[_currentOption].callback();
		}
	}

	void CMainMenuController::registerOption(const std::string& name, GUICallback cb) {
		CWidget* wdgt = Engine.getGUI().getWidget(name, true);
		CButton* bt = dynamic_cast<CButton*>(wdgt);
		if (bt) {
			_options.push_back(TOption({ bt, cb }));
		}
	}

	void CMainMenuController::setCurrentOption(int newOption) {
		for (auto& option : _options) {
			option.button->setCurrentState(CButton::EState::ST_Idle);
		}

		_currentOption = clamp(newOption, 0, static_cast<int>(_options.size()) - 1);

		_options[_currentOption].button->setCurrentState(CButton::EState::ST_Selected);
	}
}
