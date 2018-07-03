#include "mcv_platform.h"
#include "OnBlockAction.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"

OnBlockAction::OnBlockAction(Enemy* enemy, std::string animationStart, std::string animationLoop) :
	enemy(enemy),
	animationStart(animationStart),
	animationLoop(animationLoop) {
}

int OnBlockAction::execAction(float delta) {
	enemy->getSkeleton()->blendCycle(animationLoop, 0.1f, 0.1f);
	enemy->getSkeleton()->executeAction(animationStart, 0.1f, 0.1f);
	enemy->blockTimer.reset();
	enemy->isBlocking = true;
	return Leave;
}
