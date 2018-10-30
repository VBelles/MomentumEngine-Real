#pragma once

#include "gui/controllers/gui_options_menu_controller.h"

class CModuleCredits : public IModule {
private:
	std::string file = "data/gui/credits.json";
	bool active = false;
public:
	CModuleCredits(const std::string& name) : IModule(name) {}
	bool start() override;
	bool stop() override;
	void update(float delta) override;
	void render() override;
	void activate();
};
