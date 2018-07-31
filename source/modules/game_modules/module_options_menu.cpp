#include "mcv_platform.h"
#include "module_options_menu.h"
#include "gui/gui_parser.h"

#include <fstream>

bool CModuleOptionsMenu::start() {
	CApp& app = CApp::get();
	app.setResetMouse(false);

	GUI::CParser parser;
	parser.parseFile(menuFile);

	jOptions = loadJson(optionsFile);

	Engine.getGUI().activateWidget("options_menu");

	controller = new GUI::COptionsMenuController();
	controller->registerOption("invert_y_axis", jOptions["camera"]["invert_y_axis"]);
	controller->registerOption("invert_x_axis", jOptions["camera"]["invert_x_axis"]);
	controller->registerOption("fullscreen", jOptions["screen"]["fullscreen"]);
	controller->registerOption("resolution", jOptions["screen"]["resolution"]);
	controller->registerOption("vsync", jOptions["screen"]["vsync"]);
	controller->setCurrentOption(0);
	Engine.getGUI().registerController(controller);

	Engine.setInputType(InputType::Menu);

	return true;
}

bool CModuleOptionsMenu::stop() {
	jOptions["camera"]["invert_y_axis"] = controller->getOptionValue("invert_y_axis");
	jOptions["camera"]["invert_x_axis"] = controller->getOptionValue("invert_x_axis");
	jOptions["screen"]["fullscreen"] = controller->getOptionValue("fullscreen");
	jOptions["screen"]["resolution"] = controller->getOptionValue("resolution");
	jOptions["screen"]["vsync"] = controller->getOptionValue("vsync");

	std::string string = jOptions.dump(1, '\t');
	std::ofstream myfile;
	myfile.open(optionsFile);
	myfile << string;
	myfile.close();

	Engine.getGUI().unregisterController(controller);
	SAFE_DELETE(controller);
	Engine.getGUI().unregisterWidget("options_menu", true);
	return true;
}

void CModuleOptionsMenu::update(float delta) {}

void CModuleOptionsMenu::render() {}
