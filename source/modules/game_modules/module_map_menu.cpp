#include "mcv_platform.h"
#include "module_map_menu.h"
#include "gui/gui_parser.h"
#include "modules/system_modules/scripting/scripting_player.h"

bool CModuleMapMenu::start() {
	pause = false;

	GUI::CParser parser;
	parser.parseFile(menuFile);

	controller = new GUI::CMapMenuController();
	controller->registerOption("temple_teleport");
	controller->registerOption("main_entrance_teleport");
	controller->registerOption("central_field_teleport");
	controller->registerOption("tower_entrance_teleport");
	controller->registerOption("behind_statue_teleport");
	controller->registerOption("kings_graveyard_teleport");
	controller->registerOption("gate_of_doom_teleport");
	controller->registerOption("mushroom_kingdom_teleport");
	controller->registerOption("graveyard_teleport");
	controller->registerOption("behind_tower_teleport");

	return true;
}

bool CModuleMapMenu::stop() {
	Engine.getGUI().unregisterController(controller);
	safeDelete(controller);
	Engine.getGUI().unregisterWidget("map_menu", true);
	return true;
}

void CModuleMapMenu::update(float delta) {
	if (!CApp::get().isDebug()
		&& EngineInput["map"].getsPressed()
		|| pause && EngineInput["menu_back"].getsPressed()) {
		onMapButtonPressed();
	}
}

void CModuleMapMenu::render() {}

void CModuleMapMenu::onMapButtonPressed() {
	pause = !pause;

	if (pause) {
		Engine.getGUI().registerController(controller);
		controller->setCurrentOption(0);
		Engine.getGUI().activateWidget("map_menu");
		ScriptingPlayer::givePlayerControl(); //Necesario ya que se fuerza salir del debug y puede no tener el control
		CApp::get().setDebugMode(false);
		cb_globals.game_paused = 1;
	}
	else {
		Engine.getGUI().deactivateWidget("map_menu");
		Engine.getGUI().unregisterController(controller);
		cb_globals.game_paused = 0;
	}

	EngineRender.setActive(!pause);
	EngineScripting.setActive(!pause);
	Engine.getEntities().setActive(!pause);
	EngineParticles.setPaused(pause);
	CTimerFrames::get().setPaused(pause);
	CGameState* currentGamestate = EngineModules.getCurrentGameState();
	for (auto& module : *currentGamestate) {
		if (module != this) {
			module->setActive(!pause);
		}
	}

	CApp::get().setResetMouse(!pause);

}
