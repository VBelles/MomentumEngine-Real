#pragma once

#include "gui/controllers/gui_main_menu_controller.h"

class CModuleMainMenu : public IModule {
private:
	GUI::CMainMenuController* controller;
	std::string menuFile = "data/gui/main_menu.json";
	bool firstStart = true;
public:
	CModuleMainMenu(const std::string& name) : IModule(name) {}
	bool start() override;
	bool stop() override;
	void update(float delta) override;
	void render() override;
};