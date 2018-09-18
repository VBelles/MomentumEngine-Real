#pragma once

#include "modules/module_manager.h"
#include "modules/system_modules/module_render.h"
#include "modules/system_modules/module_entities.h"
#include "modules/system_modules/module_input.h"
#include "modules/game_modules/physics/module_physics.h"
#include "modules/system_modules/module_cameras.h"
#include "modules/system_modules/scripting/module_scripting.h"
#include "modules/system_modules/module_gui.h"
#include "modules/system_modules/module_navmesh.h"
#include "modules/system_modules/sound/module_sound.h"
#include "modules/system_modules/module_multithread.h"
#include "modules/system_modules/module_uniques.h"
#include "modules/system_modules/particles/module_particles.h"
#include "modules/system_modules/slash/module_slash.h"

#include <mutex>
extern std::recursive_mutex mtx;

enum InputType {
	None, Menu, Game, Console
};

struct GlobalConfig {
	VEC2 cameraAxis;
	VEC2 cameraSpeed;
	VEC2 cameraSpeedPad;
	VEC2 resolution;
	bool fullscreen;
	bool vSync;
};

class CEngine {
public:
	CEngine();
	static CEngine& get();

	bool start();
	bool stop();
	void update(float delta);
	void render();

	CModuleManager&     getModules()     { return _modules; }
	CModuleRender&      getRender()      { return _module_render; }
	CModuleEntities&    getEntities()    { return _module_entities; }
	CModuleInput&       getInput()       { return _module_input; }
	CModulePhysics&     getPhysics()     { return _module_physics; }
	CModuleCameras&     getCameras()     { return _module_cameras; }
	CModuleScripting&   getScripting()   { return _module_scripting; }
	CModuleGUI&         getGUI()         { return _module_gui; }
	CModuleNavmesh&     getNavMesh()     { return _module_navmesh; }
	CModuleSound&       getSound()       { return _module_sound; }
	CModuleMultithread& getMultithread() { return _module_multithread; }
    CModuleUniques&     getUniques()     { return _module_uniques; }
	CModuleParticles&   getParticles()	 { return _module_particles; }
	CModuleSlash&		getTrailSlash()		 { return _module_slash; }

	float getUnscaledDeltaTime() const { return current_unscaled_delta_time; }
	bool isStarted() { return started; }

	InputType getInputType();
	InputType getPreviousInputType();
	void setInputType(InputType newType);

	GlobalConfig globalConfig;

private:
	CModuleManager      _modules;
	CModuleRender       _module_render;
	CModuleEntities     _module_entities;
	CModuleInput        _module_input;
	CModulePhysics      _module_physics;
	CModuleCameras      _module_cameras;
	CModuleScripting    _module_scripting;
	CModuleGUI          _module_gui;
	CModuleNavmesh      _module_navmesh;
	CModuleSound        _module_sound;
	CModuleMultithread  _module_multithread;
	CModuleUniques      _module_uniques;
	CModuleParticles    _module_particles;
	CModuleSlash		_module_slash;

	float           current_unscaled_delta_time = 0.f;
	bool started = false;

	InputType inputType = None;
};

#define Engine          CEngine::get()
#define EngineModules   CEngine::get().getModules()
#define EngineRender    CEngine::get().getRender()
#define EngineInput     CEngine::get().getInput()
#define EnginePhysics   CEngine::get().getPhysics()
#define EngineCameras   CEngine::get().getCameras()
#define EngineScripting CEngine::get().getScripting()
#define EngineGUI       CEngine::get().getGUI()
#define EngineNavMesh   CEngine::get().getNavMesh()
#define EngineSound     CEngine::get().getSound()
#define EngineUniques   CEngine::get().getUniques()
#define EngineParticles CEngine::get().getParticles()
#define EngineSlash		CEngine::get().getTrailSlash()
