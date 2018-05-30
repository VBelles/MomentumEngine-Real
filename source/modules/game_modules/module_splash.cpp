#include "mcv_platform.h"
#include "module_splash.h"
#include "modules/module.h"

bool CModuleSplash::start() {
	_timer = 3.f;
	EngineRender.setBackgroundColor(1.f, 0.f, 0.f, 1.f);
	return true;
}

void CModuleSplash::update(float delta) {
	_timer -= delta;
	if (_timer <= 0.f) {
		EngineModules.changeGameState("main_menu");
	}
}