#pragma once

#include "modules/module_manager.h"
#include "modules/system/module_render.h"
#include "modules/system/module_entities.h"
#include "modules/game/physics/module_physics.h"
#include "modules/system/module_multithread.h"
#include "modules/system/module_input.h"
#include "modules/system/module_cameras.h"
#include "modules/system/scripting/module_scripting.h"
#include "modules/system/module_gui.h"
#include "modules/system/module_navmesh.h"
#include "modules/system/module_sound.h"

class CEngine {
public:
	CEngine();
	static CEngine& get();

	bool start();
	bool stop();
	void update(float delta);
	void render();

	CModuleManager& getModules()		 { return _modules; }
	CModuleRender& getRender()			 { return _module_render; }
	CModuleEntities& getEntities()		 { return _module_entities; }
	CModuleInput& getInput()			 { return _module_input; }
	CModulePhysics& getPhysics()		 { return _module_physics; }
	CModuleCameras& getCameras()		 { return _module_cameras; }
	CModuleScripting& getScripting()	 { return _module_scripting; }
	CModuleGUI& getGUI()				 { return _module_gui; }
	CModuleNavmesh& getNavmesh()		 { return _module_navmesh; }
	CModuleSound& getSound()			 { return _module_sound; }
	CModuleMultithread& getMultithread() { return _module_multithread; }

	float getUnscaledDeltaTime() const { return current_unscaled_delta_time; }
	bool isStarted() { return started; }

private:
	CModuleManager		_modules;
	CModuleRender		_module_render;
	CModuleEntities		_module_entities;
	CModulePhysics		_module_physics;
	CModuleInput		_module_input;
	CModuleCameras		_module_cameras;
	CModuleScripting	_module_scripting;
	CModuleGUI			_module_gui;
	CModuleNavmesh		_module_navmesh;
	CModuleSound		_module_sound;
	CModuleMultithread  _module_multithread;

	float           current_unscaled_delta_time = 0.f;
	bool started = false;
};

#define Engine CEngine::get()
#define EngineModules CEngine::get().getModules()
#define EngineInput CEngine::get().getInput()
#define EnginePhysics CEngine::get().getPhysics()
#define EngineRender CEngine::get().getRender()
#define EngineScripting CEngine::get().getScripting()
#define EngineGUI CEngine::get().getGUI()
#define EngineNavmesh CEngine::get().getNavmesh()
#define EngineSound CEngine::get().getSound()
