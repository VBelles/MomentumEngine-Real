#pragma once

#include "gui/controllers/gui_options_menu_controller.h"

class CModuleOptionsMenu : public IModule {
private:
	GUI::COptionsMenuController* controller;
	std::string menuFile = "data/gui/options_menu.json";
	std::string optionsFile = "data/configuration.json";
	json jOptions;
public:
	CModuleOptionsMenu(const std::string& name) : IModule(name) {}
	bool start() override;
	bool stop() override;
	void update(float delta) override;
	void render() override;
};
