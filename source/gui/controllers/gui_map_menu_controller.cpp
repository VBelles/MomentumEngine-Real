#include "mcv_platform.h"
#include "gui_map_menu_controller.h"
#include "gui/widgets/gui_map_marker.h"
#include "gui/widgets/gui_button.h"
#include "modules/system_modules/scripting/scripting_player.h"
#include "modules/game_modules/module_map_menu.h"

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
			&& _options[_currentOption]->overlaps(mouse.position)) {
			_options[_currentOption]->setCurrentState(CButton::EState::ST_Pressed);
		}
		else if (_currentOption >= 0 && EngineInput["menu_accept"].getsReleased()
			|| _currentOption >= 0 && EngineInput["mouse_accept"].getsReleased()
			&& _options[_currentOption]->overlaps(mouse.position)) {
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
			_options[_currentOption]->setCurrentState(CButton::EState::ST_Selected);
		}

		CEntity* playerEntity = getEntityByName(PLAYER_NAME);
		TCompTransform* playerTransform = playerEntity->get<TCompTransform>();
		VEC3 playerPos = playerTransform->getPosition();
		VEC2 playerImagePos = VEC2(playerPos.z, playerPos.x);

		float ratio = 3.41211085f;
		float ratio2 = -0.293073714;
		VEC2 vRatio = VEC2(ratio2, -ratio2);
		VEC2 offset = VEC2(1792.177, 1926.837);
		
		playerImagePos *= vRatio;
		//playerImagePos += offset;

		dbg("%f %f\n", playerImagePos.x, playerImagePos.y);

		auto playerMapWidget = Engine.getGUI().getWidget("map_player", true);
		playerMapWidget->getParams()->_position = playerImagePos;
		playerMapWidget->computeAbsolute();

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
