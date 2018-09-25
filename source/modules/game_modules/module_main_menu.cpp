#include "mcv_platform.h"
#include "module_main_menu.h"
#include "gui/gui_parser.h"

bool CModuleMainMenu::start() {
	CApp& app = CApp::get();
	app.setResetMouse(false);

	GUI::CParser parser;
	parser.parseFile(menuFile);

	Engine.getGUI().activateWidget("main_menu");

	auto startGameCB = [&]() {
		app.setResetMouse(true);
		Engine.getGUI().unregisterController(controller);
		Engine.getGUI().unregisterWidget("main_menu", true);
		EngineModules.changeGameState("game_state", true);
	};
	auto optionsMenuCB = [&]() {
		Engine.getGUI().unregisterController(controller);
		Engine.getGUI().unregisterWidget("main_menu", true);
		EngineModules.changeGameState("options_menu");
	};
	auto exitCB = []() {
		CApp::get().stopMainLoop = true;
	};

	controller = new GUI::CMainMenuController();
	controller->registerOption("start_game", startGameCB);
	controller->registerOption("options_menu", optionsMenuCB);
	controller->registerOption("exit_game", exitCB);
	controller->setCurrentOption(0);
	Engine.getGUI().registerController(controller);

	Engine.setInputType(InputType::Menu);

	if (firstStart) {
		Resources.loadResourcesBackground("data/resources.json");
		firstStart = false;
	}

	return true;
}

bool CModuleMainMenu::stop() {
	Engine.getGUI().unregisterController(controller);
	safeDelete(controller);
	Engine.getGUI().unregisterWidget("main_menu", true);
	return true;
}

void CModuleMainMenu::update(float delta) {}

void CModuleMainMenu::render() {}
