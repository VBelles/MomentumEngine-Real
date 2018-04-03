#include "mcv_platform.h"
#include "module_pause.h"

bool CModulePause::start() {
	Engine.getModules().getModule("entities")->setActive(true);
	Engine.getModules().getModule("physics")->setActive(true);
	pause = false;
	return true;
}

bool CModulePause::stop() {
	Engine.getModules().getModule("entities")->setActive(true);
	Engine.getModules().getModule("physics")->setActive(true);
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
			if (module->getName() != "pause") {
				module->setActive(!pause);
			}
		}
	}
}

void CModulePause::render() {
}
