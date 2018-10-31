#include "mcv_platform.h"
#include "gui_map_menu_controller.h"
#include "gui/widgets/gui_map_marker.h"
#include "modules/system_modules/scripting/scripting_player.h"
#include "modules/game_modules/module_map_menu.h"

namespace GUI {
	void CMapMenuController::update(float delta) {
		int option = _currentOption;
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
				if (showChrysalisOpt.button && showChrysalisOpt.button->overlaps(mouse.position)) {
					setCurrentOption(i);
				}
				else {
					setCurrentOption(_currentOption);
				}
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
			}
			else {
				if (EngineInput["menu_down"].getsPressed()) {
					setCurrentOption(_currentOption + 1);
				}
				else if (EngineInput["menu_up"].getsPressed()) {
					setCurrentOption(_currentOption - 1);
				}
			}
		}


		if (_currentOption >= 0 && EngineInput["menu_accept"].getsPressed()
			|| _currentOption >= 0 && EngineInput["mouse_accept"].getsPressed()
			&& optionOverlaps(_currentOption, mouse.position)) {
			setState(_currentOption, CButton::EState::ST_Pressed);
		}
		else if (EngineInput["menu_accept"].getsReleased()
			|| EngineInput["mouse_accept"].getsReleased() && optionOverlaps(_currentOption, mouse.position)) {
			if (_currentOption == _options.size() && showChrysalisOpt.button) {
				showChrysalisOpt.callback();
			}
			else if (_currentOption >= 0) {
				if (_options[_currentOption]->isVisible()
					&& ScriptingPlayer::isPlayerGrounded()) {
					VEC3 pos;
					UniqueElement* uniqueEvent = EngineUniques.getUniqueEvent(_options[_currentOption]->getName());
					if (uniqueEvent) {
						pos = uniqueEvent->position;
					}
					else {
						pos = _options[_currentOption]->getPos();
					}
					ScriptingPlayer::teleportPlayer(pos.x, pos.y + 1.f, pos.z);

					CModuleMapMenu* module = (CModuleMapMenu*)EngineModules.getModule(mapModule);
					if (module->isActive()) {
						module->onMapButtonPressed();
					}
				}
				else {
					//TODO
					EngineSound.emitEvent(SOUND_MENU_UNAVAILABLE);
				}
			}
			setState(_currentOption, CButton::EState::ST_Selected);
		}

		CEntity* playerEntity = getEntityByName(PLAYER_NAME);
		TCompTransform* playerTransform = playerEntity->get<TCompTransform>();
		VEC3 playerPos = playerTransform->getPosition();
		VEC2 playerImagePos = VEC2(playerPos.z, playerPos.x);

		playerImagePos *= vRatio;

		CModuleMapMenu* module = (CModuleMapMenu*)EngineModules.getModule(mapModule);
		playerImagePos += module->mapOffset;

		//dbg("%f %f\n", playerImagePos.x, playerImagePos.y);

		auto playerMapWidget = Engine.getGUI().getWidget("map_player", true);
		playerMapWidget->getParams()->_position = playerImagePos;
		playerMapWidget->computeAbsolute();
		auto mapWidget = Engine.getGUI().getWidget("map_menu", true);
		mapWidget->computeAbsolute();

		if (_currentOption != option) {
			EngineSound.emitEvent(SOUND_MENU_ROLL);
		}
	}

	void CMapMenuController::registerOption(const std::string& name, GUICallback cb) {
		if (cb) {
			CWidget* wdgt = Engine.getGUI().getWidget(name, true);
			CButton* button = dynamic_cast<CButton*>(wdgt);
			if (button) {
				showChrysalisOpt.button = button;
				showChrysalisOpt.callback = cb;
			}
		}
		else {
			CWidget* wdgt = Engine.getGUI().getWidget(name, true);
			CMapMarker* marker = dynamic_cast<CMapMarker*>(wdgt);
			if (marker) {
				_options.push_back(marker);
			}
		}
	}

	void CMapMenuController::unregisterOption(const std::string& name) {
		bool found = false;
		int i = 0;
		while (!found && i < _options.size()) {
			CWidget* opt = _options[i];
			if (opt->getName() == name) {
				found = true;
			}
			i++;
		}

		if (!found) {
			if (showChrysalisOpt.button && showChrysalisOpt.button->getName() == name) {
				showChrysalisOpt.button = nullptr;
			}
		}
		else {
			_options.erase(_options.begin() + i);
		}
	}

	void CMapMenuController::setCurrentOption(int newOption) {
		for (auto& option : _options) {
			option->setCurrentState(CButton::EState::ST_Idle);
		}

		int nOptions = 0;
		if (showChrysalisOpt.button) {
			showChrysalisOpt.button->setCurrentState(CButton::EState::ST_Idle);
			nOptions = static_cast<int>(_options.size());
			_currentOption = (newOption + nOptions + 1) % (nOptions + 1);
		}
		else {
			nOptions = static_cast<int>(_options.size());
			_currentOption = (newOption + nOptions) % (nOptions);
		}

		if (showChrysalisOpt.button && _currentOption == nOptions) {
			showChrysalisOpt.button->setCurrentState(CButton::EState::ST_Selected);
		}
		else {
			_options[_currentOption]->setCurrentState(CButton::EState::ST_Selected);
		}
	}

	void CMapMenuController::setState(int option, CButton::EState state) {
		if (showChrysalisOpt.button && option == _options.size()) {
			showChrysalisOpt.button->setCurrentState(state);
		}
		else if (option >= 0 && option < _options.size()) {
			_options[option]->setCurrentState(state);
		}
	}

	bool CMapMenuController::optionOverlaps(int option, VEC2 pos) {
		bool ret = false;

		if (showChrysalisOpt.button && option == _options.size()) {
			ret = showChrysalisOpt.button->overlaps(pos);
		}
		else if (option >= 0 && option < _options.size()) {
			ret = _options[option]->overlaps(pos);
		}

		return ret;
	}
}
