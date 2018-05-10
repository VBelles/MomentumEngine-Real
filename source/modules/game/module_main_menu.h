#pragma once

#include "gui/controllers/gui_main_menu_controller.h"

class CModuleMainMenu : public IModule {
private:
	GUI::CMainMenuController* controller;

public:
	CModuleMainMenu(const std::string& name) : IModule(name) {}
	bool start() override;
	bool stop() override;
	void update(float delta) override;
	void render() override;
};