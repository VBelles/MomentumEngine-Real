#include "mcv_platform.h"
#include "OnVerticalLaunchAction.h"
#include "components/comp_transform.h"
#include "skeleton/comp_skeleton.h"

OnVerticalLaunchAction::OnVerticalLaunchAction(Enemy* enemy, std::string animation) :
	enemy(enemy),
	animation(animation) {
}

int OnVerticalLaunchAction::execAction(float delta) {
	enemy->getSkeleton()->executeAction(animation, 0.1f, 0.1f);
	enemy->stunDuration = enemy->receivedAttack.verticalLauncher->suspensionDuration;
	enemy->velocity = enemy->receivedAttack.verticalLauncher->velocity;
	enemy->initialLaunchPos = enemy->getTransform()->getPosition();
	return Leave;
}
