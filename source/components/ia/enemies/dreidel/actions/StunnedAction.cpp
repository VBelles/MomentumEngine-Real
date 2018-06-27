#include "mcv_platform.h"
#include "StunnedAction.h"

#include "skeleton/comp_skeleton.h"

StunnedAction::StunnedAction(Enemy* enemy): enemy(enemy) {
}

int StunnedAction::execAction(float delta) {
	enemy->updateGravity(delta);
	if (enemy->stunTimer.elapsed() > enemy->stunDuration) {
		enemy->isStunned = false;
		enemy->getSkeleton()->setTimeFactor(1);
		return Leave;
	}
	else {
		return Stay;
	}
}
