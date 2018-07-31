#include "mcv_platform.h"
#include "module_options_menu.h"
#include "gui/gui_parser.h"

bool CModuleOptionsMenu::start() {
	CApp& app = CApp::get();
	app.setResetMouse(false);
	
	GUI::CParser parser;
	parser.parseFile(menuFile);

	Engine.getGUI().activateWidget("options_menu");

	auto cB = []() {
	};
	//auto mainMenuCB = []() {
	//	EngineModules.changeGameState("main_menu");
	//};
	//auto exitCB = []() {
	//	CApp::get().stopMainLoop = true;
	//};

	controller = new GUI::COptionsMenuController();
	controller->registerOption("invert_y_axis", cB);
	controller->registerOption("invert_x_axis", cB);
	controller->setCurrentOption(0);
	Engine.getGUI().registerController(controller);

	Engine.setInputType(InputType::Menu);

	return true;
}

bool CModuleOptionsMenu::stop() {
	Engine.getGUI().unregisterController(controller);
	SAFE_DELETE(controller);
	Engine.getGUI().unregisterWidget("main_menu", true);
	return true;
}

void CModuleOptionsMenu::update(float delta) {}

void CModuleOptionsMenu::render() {}
