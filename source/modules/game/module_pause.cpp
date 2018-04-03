#include "mcv_platform.h"
#include "module_pause.h"

bool CModulePause::start() {
	pause = false;
	return true;
}

bool CModulePause::stop() {
	pause = false;
	return true;
}

void CModulePause::update(float delta) {
	if (EngineInput["pause"].getsPressed()) {
		pause = !pause;
		Engine.getModules().getModule("entities")->setActive(!pause);
		Engine.getModules().getModule("physics")->setActive(!pause);

		CGameState* currentGamestate = Engine.getModules().getCurrentGameState();
		for (auto& module : *currentGamestate) {
			if (module != this) {
				module->setActive(!pause);
			}
		}
	}
}

void CModulePause::render() {
}
