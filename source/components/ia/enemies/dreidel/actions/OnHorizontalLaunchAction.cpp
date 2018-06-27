#include "mcv_platform.h"
#include "OnHorizontalLaunchAction.h"

#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"

OnHorizontalLaunchAction::OnHorizontalLaunchAction(Enemy* enemy): enemy(enemy) {
}

int OnHorizontalLaunchAction::execAction(float delta) {
	enemy->getSkeleton()->setTimeFactor(0);
	enemy->floatingDuration = enemy->receivedAttack.horizontalLauncher->suspensionDuration;
	enemy->initialLaunchPos = enemy->getTransform()->getPosition();
	enemy->velocity = enemy->receivedAttack.horizontalLauncher->velocity;
	return Leave;
}
