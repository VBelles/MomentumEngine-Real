#pragma once

#include "gui/controllers/gui_main_menu_controller.h"

class CModulePause : public IModule {
private:
	bool pause = false;
	GUI::CMainMenuController* controller;

public:
	CModulePause(const std::string& aname) : IModule(aname) {}
    bool start() override;
    bool stop() override;
    void update(float delta) override;
    void render() override;
	void onPausePressed();
};