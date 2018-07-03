#include "mcv_platform.h"
#include "OnStunAction.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"

OnStunAction::OnStunAction(Enemy* enemy, std::string animationStart, std::string animationLoop) :
	enemy(enemy),
	animationStart(animationStart),
	animationLoop(animationLoop) {
}

int OnStunAction::execAction(float delta) {
	enemy->getSkeleton()->blendCycle(animationLoop, 0.1f, 0.1f);
	enemy->getSkeleton()->executeAction(animationStart, 0.1f, 0.1f);
	return Leave;
}
