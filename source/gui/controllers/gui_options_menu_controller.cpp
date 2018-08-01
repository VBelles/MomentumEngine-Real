#include "mcv_platform.h"
#include "gui_options_menu_controller.h"
#include "gui/widgets/gui_option.h"
#include "gui/widgets/gui_button.h"

namespace GUI {
	void COptionsMenuController::update(float delta) {
		auto& mouse = EngineInput[Input::PLAYER_1].mouse();
		if (mouse.position_delta.Length() > 0) {
			_options[_currentOption]->getPreviousButton()->setCurrentState(CButton::EState::ST_Idle);
			_options[_currentOption]->getNextButton()->setCurrentState(CButton::EState::ST_Idle);
			int i = 0;
			while (i < _options.size()) {
				COption* option = _options[i];
				if (option->overlaps(mouse.position)) {
					if (i != _currentOption) {
						setCurrentOption(i);
					}
					if (option->getPreviousButton()->overlaps(mouse.position)) {
						option->getPreviousButton()->setCurrentState(CButton::EState::ST_Selected);
					}
					else if (option->getNextButton()->overlaps(mouse.position)) {
						option->getNextButton()->setCurrentState(CButton::EState::ST_Selected);
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
				else if (leftAnalogInput.x < -0.5) {
					_options[_currentOption]->setCurrentOption(_options[_currentOption]->getCurrentOption() - 1);
				}
				else if (leftAnalogInput.x > 0.5) {
					_options[_currentOption]->setCurrentOption(_options[_currentOption]->getCurrentOption() + 1);
				}
			}
			else {
				if (EngineInput["menu_down"].getsPressed()) {
					setCurrentOption(_currentOption + 1);
				}
				else if (EngineInput["menu_up"].getsPressed()) {
					setCurrentOption(_currentOption - 1);
				}
				else if (EngineInput["menu_left"].getsPressed()) {
					_options[_currentOption]->setCurrentOption(_options[_currentOption]->getCurrentOption() - 1);
				}
				else if (EngineInput["menu_right"].getsPressed()) {
					_options[_currentOption]->setCurrentOption(_options[_currentOption]->getCurrentOption() + 1);
				}
			}
		}

		if (_currentOption >= 0 && EngineInput["mouse_accept"].getsPressed()
			&& _options[_currentOption]->overlaps(mouse.position)) {
			if (_options[_currentOption]->getPreviousButton()->overlaps(mouse.position)) {
				_options[_currentOption]->getPreviousButton()->setCurrentState(CButton::EState::ST_Pressed);
			}
			else if (_options[_currentOption]->getNextButton()->overlaps(mouse.position)) {
				_options[_currentOption]->getNextButton()->setCurrentState(CButton::EState::ST_Pressed);
			}
		}
		else if (_currentOption >= 0 && EngineInput["mouse_accept"].getsReleased()
			&& _options[_currentOption]->overlaps(mouse.position)) {
			if (_options[_currentOption]->getPreviousButton()->overlaps(mouse.position)) {
				_options[_currentOption]->getPreviousButton()->setCurrentState(CButton::EState::ST_Selected);
				_options[_currentOption]->setCurrentOption(_options[_currentOption]->getCurrentOption() - 1);
			}
			else if (_options[_currentOption]->getNextButton()->overlaps(mouse.position)) {
				_options[_currentOption]->getNextButton()->setCurrentState(CButton::EState::ST_Selected);
				_options[_currentOption]->setCurrentOption(_options[_currentOption]->getCurrentOption() + 1);
			}
		}

		if (EngineInput["menu_back"].getsReleased()) {
			EngineModules.changeGameState("main_menu");
		}
	}

	void COptionsMenuController::registerOption(const std::string& name, const json& value) {
		CWidget* wdgt = Engine.getGUI().getWidget(name, true);
		COption* op = dynamic_cast<COption*>(wdgt);
		if (op) {
			_options.push_back(op);
		}
		if (!value.empty()) {
			op->setCurrentValue(value);
		}
	}

	void COptionsMenuController::setCurrentOption(int newOption) {
		for (auto& option : _options) {
			option->setCurrentState(COption::EState::ST_Idle);
		}

		int nOptions = static_cast<int>(_options.size());
		_currentOption = (newOption + nOptions) % nOptions;

		_options[_currentOption]->setCurrentState(COption::EState::ST_Selected);
	}

	json COptionsMenuController::getOptionValue(const std::string& name) {
		CWidget* wdgt = Engine.getGUI().getWidget(name, true);
		COption* op = dynamic_cast<COption*>(wdgt);
		json ret = json();
		if (op) {
			ret = op->getCurrentOptionValue();
		}
		return ret;
	}
}
