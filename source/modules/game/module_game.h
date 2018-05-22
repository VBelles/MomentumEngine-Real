#pragma once

#include "gui/controllers/hud_controller.h"

class CModuleGame : public IModule {
private:
	GUI::CHudController* hudController;

public:
	CModuleGame(const std::string& aname) : IModule(aname) {}
	bool start() override;
	bool stop() override;
	void update(float delta) override;
	void render() override;
};
