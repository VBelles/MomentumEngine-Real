#include "mcv_platform.h"
#include "OnStunAction.h"

#include "skeleton/comp_skeleton.h"

OnStunAction::OnStunAction(Enemy* enemy): enemy(enemy) {
}

int OnStunAction::execAction(float delta) {
	enemy->getSkeleton()->setTimeFactor(0);
	enemy->stunDuration = enemy->receivedAttack.stun->duration;
	enemy->stunTimer.reset();
	enemy->isStunned = true;
	return Leave;
}
