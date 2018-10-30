#pragma once

#include "gui/controllers/gui_options_menu_controller.h"

class CModuleControlsHelp : public IModule {
private:
	std::string file = "data/gui/controls.json";
	bool active = false;
public:
	CModuleControlsHelp(const std::string& name) : IModule(name) {}
	bool start() override;
	bool stop() override;
	void update(float delta) override;
	void render() override;
	void activate();
};
