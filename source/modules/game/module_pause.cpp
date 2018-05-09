#include "mcv_platform.h"
#include "module_pause.h"
#include "gui/gui_parser.h"

bool CModulePause::start() {
	pause = false;

	GUI::CParser parser;
	parser.parseFile("data/gui/test_pause_menu.json");

	auto resumeGameCB = [&] {
		onPausePressed(); 
	};
	auto mainMenuCB = [&]() {
		EngineScripting.setActive(true);
		Engine.getEntities().setActive(true);
		CGameState* currentGamestate = EngineModules.getCurrentGameState();
		for (auto& module : *currentGamestate) {
			if (module != this) {
				module->setActive(true);
			}
		}
		EngineModules.changeGameState("main_menu");
	};
	auto exitCB = []() {
		CApp::get().stopMainLoop = true;
	};

	controller = new GUI::CMainMenuController();
	controller->registerOption("resume_game", resumeGameCB);
	controller->registerOption("main_menu", mainMenuCB);
	controller->registerOption("exit_game", exitCB);

	return true;
}

bool CModulePause::stop() {
	Engine.getGUI().unregisterController(controller);
	SAFE_DELETE(controller);
	Engine.getGUI().unregisterWidget("test_pause_menu", true);
	//ShowCursor(false);
	//CApp::get().resetCursorPos = true;
	return true;
}

void CModulePause::update(float delta) {
	if (EngineInput["pause"].getsPressed()) {
		onPausePressed();
	}
}

void CModulePause::onPausePressed() {
	pause = !pause;

	if (pause) {
		Engine.getGUI().registerController(controller);
		controller->setCurrentOption(0);
		Engine.getGUI().activateWidget("test_pause_menu");
	}
	else {
		Engine.getGUI().deactivateWidget("test_pause_menu");
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

	//ShowCursor(pause);
	//CApp::get().resetCursorPos = !pause;
}

void CModulePause::render() {}
