#include "mcv_platform.h"
#include "OnVerticalLaunchAction.h"

#include "skeleton/comp_skeleton.h"

OnVerticalLaunchAction::OnVerticalLaunchAction(Enemy* enemy): enemy(enemy) {
}

int OnVerticalLaunchAction::execAction(float delta) {
	enemy->getSkeleton()->setTimeFactor(0);
	enemy->floatingDuration = enemy->receivedAttack.verticalLauncher->suspensionDuration;
	enemy->velocity = enemy->receivedAttack.verticalLauncher->velocity;
	return Leave;
}
