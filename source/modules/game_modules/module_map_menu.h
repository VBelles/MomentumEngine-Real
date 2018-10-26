#pragma once

#include "gui/controllers/gui_map_menu_controller.h"

class CModuleMapMenu : public IModule {
private:
	GUI::CMapMenuController* controller;
	std::string menuFile = "data/gui/map_menu.json";
	bool pause = false;
	bool blocked = false;

	void showChrysalisesCallback();
	void showChrysalises();
	void hideCrysalises();

	int coinsToSpend = 72;

public:
	CModuleMapMenu(const std::string& name) : IModule(name) {}
	bool start() override;
	bool stop() override;
	void update(float delta) override;
	void render() override;
	void onMapButtonPressed();
	void setBlocked(bool blocked);
	bool isBlocked();
};
