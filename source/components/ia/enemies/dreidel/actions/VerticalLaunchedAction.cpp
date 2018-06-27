#include "mcv_platform.h"
#include "VerticalLaunchedAction.h"

#include "skeleton/comp_skeleton.h"

VerticalLaunchedAction::VerticalLaunchedAction(Enemy* enemy): enemy(enemy) {
}

int VerticalLaunchedAction::execAction(float delta) {
	enemy->updateGravity(delta);
	if (enemy->velocity.y <= 0) {
		enemy->timer.reset();
		enemy->getSkeleton()->setTimeFactor(1);
		return Leave;
	}
	else {
		return Stay;
	}
}
