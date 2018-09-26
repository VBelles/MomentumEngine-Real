#pragma once

#include "module.h"
#include "gamestate.h"

class CModuleManager {
public:
	bool start();
	bool stop();
	void update(float delta);
	void render();
	void renderGUI();

	void registerSystemModule(IModule* mod);
	void registerGameModule(IModule* mod);
	IModule* getModule(const std::string& gsName);

	void registerGameState(CGameState* gs);
	void changeGameState(const std::string& gsName, bool loadingScreen = false);
	CGameState* getGameState(const std::string& gsName);
	CGameState* getCurrentGameState();

	void loadModules(const std::string& filename);
	void loadGamestates(const std::string& filename);

	LRESULT OnOSMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	void applyRequestedGameState();
	void applyGameState();
	void renderDebug();

	bool startModule(IModule* mod);
	bool stopModule(IModule* mod);
	bool startModules(VModules& modules);
	bool stopModules(VModules& modules);

	VModules _registered_modules;
	VModules _system_modules;
	VModules _update_modules;
	VModules _render_modules;

	VGameStates _gamestates;
	CGameState* _current_gs = nullptr;
	CGameState* _startup_gs = nullptr;
	CGameState* _requested_gs = nullptr;

	bool useLoadingScreen = false;
	volatile bool gameStateChanged = false;
	std::string loadingScreenFile = "data/gui/loading_screen.json";
	std::string loadingScreenWdgt = "loading_screen";
};
