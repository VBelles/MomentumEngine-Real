#include "mcv_platform.h"
#include "module_gameover.h"

bool CModuleGameOver::start() {
    EngineRender.setBackgroundColor(0.f, 0.f, 1.f, 1.f);
    return true;
}

void CModuleGameOver::update(float delta) {
    if (isPressed(VK_ESCAPE)) {
        EngineModules.changeGameState("main_menu");
    }
}
