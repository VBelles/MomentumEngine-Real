#include "mcv_platform.h"
#include "module_map_menu.h"
#include "gui/gui_parser.h"
#include "modules/system_modules/scripting/scripting_player.h"

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

	Engine.setInputType(InputType::Menu);

	return true;
}

bool CModuleMapMenu::stop() {
	Engine.getGUI().unregisterController(controller);
	SAFE_DELETE(controller);
	Engine.getGUI().unregisterWidget("map_menu", true);
	return true;
}

void CModuleMapMenu::update(float delta) {
	if (EngineInput["map"].getsPressed()) {
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
	}
	else {
		Engine.getGUI().deactivateWidget("map_menu");
		Engine.getGUI().unregisterController(controller);
	}

	EngineScripting.setActive(!pause);
	Engine.getEntities().setActive(!pause);
	CGameState* currentGamestate = EngineModules.getCurrentGameState();
	for (auto& module : *currentGamestate) {
		if (module != this) {
			module->setActive(!pause);
		}
	}

	CApp::get().setResetMouse(!pause);

}