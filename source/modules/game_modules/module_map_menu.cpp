#include "mcv_platform.h"
#include "module_map_menu.h"
#include "gui/gui_parser.h"
#include "modules/system_modules/scripting/scripting_player.h"
#include "modules/system_modules/sound/comp_music.h"

bool CModuleMapMenu::start() {
	pause = false;

	GUI::CParser parser;
	parser.parseFile(menuFile);

	controller = new GUI::CMapMenuController();
	controller->registerOption("Tutorial");
	controller->registerOption("Origin");
	controller->registerOption("Inicio_cementerio");
	controller->registerOption("Derecha_cementerio");
	controller->registerOption("Primer_altar");
	controller->registerOption("Pie_faro");

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
		TCompMusic* music = static_cast<CEntity*>(getEntityByName(MUSIC_PLAYER))->get<TCompMusic>();
		music->setPauseMenu(true);
	}
	else {
		Engine.getGUI().deactivateWidget("map_menu");
		Engine.getGUI().unregisterController(controller);
		cb_globals.game_paused = 0;
		TCompMusic* music = static_cast<CEntity*>(getEntityByName(MUSIC_PLAYER))->get<TCompMusic>();
		music->setPauseMenu(false);
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
