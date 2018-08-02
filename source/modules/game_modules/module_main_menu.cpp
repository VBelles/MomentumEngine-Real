#include "mcv_platform.h"
#include "module_main_menu.h"
#include "gui/gui_parser.h"

bool CModuleMainMenu::start() {
	CApp& app = CApp::get();
	app.setResetMouse(false);
	
	GUI::CParser parser;
	parser.parseFile(menuFile);

	Engine.getGUI().activateWidget("main_menu");

	auto startGameCB = []() {
		EngineModules.changeGameState("game_state");
	};
	auto optionsMenuCB = []() {
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

	return true;
}

bool CModuleMainMenu::stop() {
	Engine.getGUI().unregisterController(controller);
	SAFE_DELETE(controller);
	Engine.getGUI().unregisterWidget("main_menu", true);
	return true;
}

void CModuleMainMenu::update(float delta) {}

void CModuleMainMenu::render() {}
