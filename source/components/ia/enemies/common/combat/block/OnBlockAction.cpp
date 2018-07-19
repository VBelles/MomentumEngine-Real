#include "mcv_platform.h"
#include "OnBlockAction.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_give_power.h"

REGISTER_BTACTION("OnBlockAction", OnBlockAction);

OnBlockAction::OnBlockAction(Enemy* enemy, std::string animationStart, std::string animationLoop) :
	enemy(enemy),
	animationStart(animationStart),
	animationLoop(animationLoop) {
}

int OnBlockAction::execAction(float delta) {
	enemy->getSkeleton()->blendCycle(animationLoop, 0.1f, 0.1f);
	enemy->getSkeleton()->executeAction(animationStart, 0.1f, 0.1f);
	enemy->getPower()->setStateMultiplier(0.01f);
	enemy->blockTimer.reset();
	enemy->isBlocking = true;
	return Leave;
}

void OnBlockAction::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animationStart = j.value("animation_start", animationStart);
	animationLoop = j.value("animation_loop", animationLoop);
}