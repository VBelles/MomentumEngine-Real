#include "mcv_platform.h"
#include "module_map_menu.h"
#include "gui/gui_parser.h"
#include "modules/system_modules/scripting/scripting_player.h"
#include "modules/system_modules/sound/music_player.h"
#include "modules/game_modules/game/module_game.h"
#include "components/player/comp_player_model.h"

bool CModuleMapMenu::start() {
	pause = false;
	blocked = false;

	GUI::CParser parser;
	parser.parseFile(menuFile);

	auto showChrysalisCB = [&] {
		showChrysalisesCallback();
	};

	controller = new GUI::CMapMenuController();
	controller->registerOption("temple_teleport");
	controller->registerOption("main_entrance_teleport");
	controller->registerOption("central_field_teleport");
	controller->registerOption("tower_entrance_teleport");
	controller->registerOption("behind_statue_teleport");
	controller->registerOption("kings_graveyard_teleport");
	controller->registerOption("mushroom_kingdom_teleport");
	controller->registerOption("graveyard_teleport");
	controller->registerOption("behind_tower_teleport");
	controller->registerOption("gate_of_doom_teleport");

	controller->registerOption("show_chrysalis_button", showChrysalisCB);

	UniqueElement* unique = EngineUniques.getUniqueEvent("chrysalis_help");
	if (unique && unique->done) {
		controller->unregisterOption("show_chrysalis_button");
		EngineGUI.getWidget("show_chrysalis_button", true)->getParams()->_visible = false;
		showChrysalises();
	}
	else {
		hideCrysalises();
	}

	return true;
}

bool CModuleMapMenu::stop() {
	Engine.getGUI().unregisterController(controller);
	safeDelete(controller);
	Engine.getGUI().unregisterWidget("map_menu", true);
	blocked = false;
	return true;
}

void CModuleMapMenu::update(float delta) {
	if (!blocked && /*!CApp::get().isDebug() &&*/ EngineInput["map"].getsPressed()
		|| pause && EngineInput["menu_back"].getsPressed()) {
		onMapButtonPressed();
	}
}

void CModuleMapMenu::render() {
	//dbg("Render map menu\n");
	if (CApp::get().isDebug()) {
		//if (ImGui::TreeNode("Map menu")) {
			ImGui::DragFloat2("Map offset", &mapOffset.x, 1.f, -1000.f, 1000.f);
		//}
	}
}

void CModuleMapMenu::onMapButtonPressed() {
	if (!pause && !ScriptingPlayer::isPlayerGrounded()) {
		EngineSound.emitEvent(SOUND_MENU_BACK);
		return;
	}
	pause = !pause;

	if (pause) {
		Engine.getGUI().registerController(controller);
		controller->setCurrentOption(0);
		Engine.getGUI().activateWidget("map_menu");
		ScriptingPlayer::givePlayerControl(); //Necesario ya que se fuerza salir del debug y puede no tener el control
		//CApp::get().setDebugMode(false);
		cb_globals.game_paused = 1;
		EngineSound.getMusicPlayer()->setPauseMenu(true);
	}
	else {
		Engine.getGUI().deactivateWidget("map_menu");
		Engine.getGUI().unregisterController(controller);
		cb_globals.game_paused = 0;
		EngineSound.getMusicPlayer()->setPauseMenu(false);
		EngineSound.emitEvent(SOUND_MENU_BACK);
	}

	EngineGame->showHUD(!pause);
	EngineGUI.setDialogActive(!pause);
	EngineRender.setActive(!pause);
	EngineScripting.setActive(!pause);
	Engine.getEntities().setActive(!pause);
	EngineParticles.setPaused(pause);
	EngineSound.setPaused(pause);
	CTimerFrames::get().setPaused(pause);
	CGameState* currentGamestate = EngineModules.getCurrentGameState();
	for (auto& module : *currentGamestate) {
		if (module != this) {
			module->setActive(!pause);
		}
	}

	CApp::get().setResetMouse(!pause);
}

void CModuleMapMenu::setBlocked(bool blocked) {
	this->blocked = blocked;
}

bool CModuleMapMenu::isBlocked() {
	return blocked;
}

void CModuleMapMenu::showChrysalisesCallback() {
	CEntity* playerEntity = getEntityByName(PLAYER_NAME);
	TCompPlayerModel* playerModel = playerEntity->get<TCompPlayerModel>();
	TCompCollectableManager* collectableManager = playerModel->get<TCompCollectableManager>();
	if (collectableManager->spendCoins(coinsToSpend)) {
		controller->unregisterOption("show_chrysalis_button");
		EngineGUI.getWidget("show_chrysalis_button", true)->getParams()->_visible = false;
		showChrysalises();
		UniqueElement* unique = EngineUniques.getUniqueEvent("chrysalis_help");
		if (unique) unique->done = true;
	}
	else {
		EngineSound.emitEvent(SOUND_MENU_UNAVAILABLE);
	}
}

void CModuleMapMenu::showChrysalises() {
	GUI::CWidget* wdgt = EngineGUI.getWidget("chrysalises_map_menu", true);
	if (wdgt) {
		for (GUI::CWidget* child : wdgt->getChildren()) {
			if (!EngineUniques.getUniqueChrysalis(child->getName())->done) {
				child->getParams()->_visible = true;
			}
		}
	}
}

void CModuleMapMenu::hideCrysalises() {
	GUI::CWidget* wdgt = EngineGUI.getWidget("chrysalises_map_menu", true);
	if (wdgt) {
		for (GUI::CWidget* child : wdgt->getChildren()) {
			child->getParams()->_visible = false;
		}
	}
}