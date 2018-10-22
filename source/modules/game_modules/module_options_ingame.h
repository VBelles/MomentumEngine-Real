#pragma once

#include "gui/controllers/gui_options_menu_controller.h"

class CModuleOptionsIngame : public IModule {
private:
	GUI::COptionsMenuController* controller;
	std::string menuFile = "data/gui/options_ingame.json";
	std::string optionsFile = "data/configuration.json";
	json jOptions;
	bool active = false;
	void activate(bool newActive);
public:
	CModuleOptionsIngame(const std::string& name) : IModule(name) {}
	bool start() override;
	bool stop() override;
	void update(float delta) override;
	void render() override;
	void activate();
};
