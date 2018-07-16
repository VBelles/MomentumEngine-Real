#include "mcv_platform.h"
#include "OnHorizontalLaunchAction.h"
#include "components/comp_transform.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"

OnHorizontalLaunchAction::OnHorizontalLaunchAction(Enemy* enemy, std::string animation) :
	enemy(enemy),
	animation(animation) {
}

int OnHorizontalLaunchAction::execAction(float delta) {
	enemy->getSkeleton()->executeAction(animation, 0.1f, 0.1f);
	enemy->stunDuration = enemy->receivedAttack.horizontalLauncher->suspensionDuration;
	enemy->velocity = enemy->receivedAttack.horizontalLauncher->velocity;
	enemy->initialLaunchPos = enemy->getTransform()->getPosition();
	enemy->isBlocking = false;
	enemy->blockingBroken = false;
	//TODO aqu� deber�a poner su gravedad a 0.6 de la normal
	return Leave;
}
