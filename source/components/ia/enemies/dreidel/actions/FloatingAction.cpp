#include "mcv_platform.h"
#include "FloatingAction.h"

#include "skeleton/comp_skeleton.h"

FloatingAction::FloatingAction(Enemy* enemy): enemy(enemy) {
}

int FloatingAction::execAction(float delta) {
	enemy->getSkeleton()->setTimeFactor(0);
	if (enemy->timer.elapsed() > enemy->floatingDuration) {
		enemy->getSkeleton()->setTimeFactor(1);
		return Leave;
	}
	else {
		return Stay;
	}
}
