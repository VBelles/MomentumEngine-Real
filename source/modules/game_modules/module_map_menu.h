#pragma once

#include "gui/controllers/gui_map_menu_controller.h"

class CModuleMapMenu : public IModule {
private:
	GUI::CMapMenuController* controller;
	std::string menuFile = "data/gui/map_menu.json";
	bool pause = false;
	bool blocked = false;

public:
	VEC2 mapOffset = VEC2(0, 0);

	CModuleMapMenu(const std::string& name) : IModule(name) {}
	bool start() override;
	bool stop() override;
	void update(float delta) override;
	void render() override;
	void onMapButtonPressed();
	void setBlocked(bool blocked);
	bool isBlocked();
};
