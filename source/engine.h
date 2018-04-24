#pragma once

#include "modules/module_manager.h"
#include "modules/system/module_render.h"
#include "modules/game/module_entities.h"
#include "modules/game/physics/module_physics.h"
#include "modules/system/module_input.h"
#include "modules/system/module_cameras.h"
#include "modules/system/module_fsm.h"
#include "modules/game/scripting/module_scripting.h"

class CEngine {
public:
	CEngine();
	static CEngine& get();

	bool start();
	bool stop();
	void update(float delta);
	void render();

	CModuleManager& getModules() { return _modules; }
	CModuleRender& getRender() { return _module_render; }
	CModuleEntities& getEntities() { return _module_entities; }
	CModuleInput& getInput() { return _module_input; }
	CModulePhysics& getPhysics() { return _module_physics; }
	CModuleCameras& getCameras() { return _module_cameras; }
	CModuleFSM& getFSM() { return _module_fsm; }
	CModuleScripting& getScripting() { return _module_scripting; }

	float getUnscaledDeltaTime() const { return current_unscaled_delta_time; }
	bool isStarted() { return started; }

private:
	CModuleManager   _modules;
	CModuleRender    _module_render;
	CModuleEntities  _module_entities;
	CModulePhysics   _module_physics;
	CModuleInput     _module_input;
	CModuleCameras   _module_cameras;
	CModuleFSM       _module_fsm;
	CModuleScripting _module_scripting;

	float           current_unscaled_delta_time = 0.f;
	bool started = false;
};

#define Engine CEngine::get()
#define EngineInput CEngine::get().getInput()
#define EnginePhysics CEngine::get().getPhysics()
#define EngineRender CEngine::get().getRender()
#define EngineScripting CEngine::get().getScripting()