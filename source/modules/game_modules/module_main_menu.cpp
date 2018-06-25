#include "mcv_platform.h"
#include "module_main_menu.h"
#include "gui/gui_parser.h"

bool CModuleMainMenu::start() {
	CApp& app = CApp::get();
	app.setResetMouse(false);
	
	GUI::CParser parser;
	parser.parseFile("data/gui/test.json");

	Engine.getGUI().activateWidget("test");

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

	Engine.setInputType(InputType::Menu);

	return true;
}

bool CModuleMainMenu::stop() {
	Engine.getGUI().unregisterController(controller);
	SAFE_DELETE(controller);
	Engine.getGUI().unregisterWidget("test", true);
	return true;
}

void CModuleMainMenu::update(float delta) {}

void CModuleMainMenu::render() {}
