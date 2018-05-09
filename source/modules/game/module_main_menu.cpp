#include "mcv_platform.h"
#include "module_main_menu.h"
#include "gui/gui_parser.h"

bool CModuleMainMenu::start() {
	//ShowCursor(true);
	//CApp::get().resetCursorPos = false;

	GUI::CParser parser;
	parser.parseFile("data/gui/test.json");

	Engine.getGUI().activateWidget("test");

	Engine.getGUI().getVariables().setVariant("progress", 0.5f);

	auto startGameCB = []() {
		EngineModules.changeGameState("game_state");
	};
	auto exitCB = []() {
		CApp::get().stopMainLoop = true;
	};

	controller = new GUI::CMainMenuController();
	controller->registerOption("start_game", startGameCB);
	controller->registerOption("exit_game", exitCB);
	controller->setCurrentOption(0);
	Engine.getGUI().registerController(controller);

	return true;
}

bool CModuleMainMenu::stop() {
	Engine.getGUI().unregisterController(controller);
	SAFE_DELETE(controller);
	Engine.getGUI().unregisterWidget("test", true);
	//CApp::get().resetCursorPos = true;
	//ShowCursor(false);
	return true;
}

void CModuleMainMenu::update(float delta) {}

void CModuleMainMenu::render() {}
