#include "mcv_platform.h"
#include "engine.h"
#include "modules/game_modules/module_splash.h"
#include "modules/game_modules/module_main_menu.h"
#include "modules/game_modules/module_gameover.h"
#include "modules/game_modules/game/module_game.h"
#include "modules/game_modules/module_pause.h"
#include "modules/test/module_test_instancing.h"

CEngine& CEngine::get() {
	static CEngine engine;
	return engine;
}

CEngine::CEngine()
	: _module_render("render")
	, _module_entities("entities")
	, _module_physics("physics")
	, _module_multithread("multithread")
	, _module_input("input")
	, _module_cameras("cameras")
	, _module_scripting("scripting")
	, _module_gui("gui")
	, _module_navmesh("navmesh")
	, _module_sound("sound")
	, _module_uniques("uniques")
    , _module_instancing("instancing")
    , _module_particles("particles") {
}

bool CEngine::start() {
	static CModuleSplash   module_splash("splash");
	static CModuleMainMenu module_main_menu("main_menu");
	static CModuleGameOver module_game_over("game_over");
	static CModuleGame module_test_axis("game");
	static CModulePause module_pause("pause");
	static CModuleTestInstancing module_test_instancing("test_instancing");

	_modules.registerSystemModule(&_module_entities);
	_modules.registerSystemModule(&_module_render);
	_modules.registerSystemModule(&_module_input);
	_modules.registerSystemModule(&_module_scripting);
	_modules.registerSystemModule(&_module_gui);
	_modules.registerSystemModule(&_module_navmesh);
	_modules.registerSystemModule(&_module_sound);
	_modules.registerSystemModule(&_module_multithread);
	_modules.registerSystemModule(&_module_uniques);
	_modules.registerSystemModule(&_module_instancing);
    _modules.registerSystemModule(&_module_particles);

	_modules.registerGameModule(&_module_physics);
	_modules.registerGameModule(&module_splash);
	_modules.registerGameModule(&module_main_menu);
	_modules.registerGameModule(&module_game_over);
	_modules.registerGameModule(&module_test_axis);
	_modules.registerGameModule(&_module_cameras);
	_modules.registerGameModule(&module_pause);
	_modules.registerGameModule(&module_test_instancing);

	_modules.loadModules("data/modules.json");
	_modules.loadGamestates("data/gamestates.json");

	started = _modules.start();
	return started;
}

bool CEngine::stop() {
	bool ok = true;
	ok &= _modules.stop();
	return ok;
}

void CEngine::update(float delta) {
	PROFILE_FUNCTION("CEngine::update");
	current_unscaled_delta_time = delta;
	_modules.update(delta);
}

void CEngine::render() {
	PROFILE_FUNCTION("CEngine::render");
	_module_render.generateFrame();
}
