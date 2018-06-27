#include "mcv_platform.h"
#include "IdleWarAction.h"

#include "components/comp_transform.h"
#include "skeleton/comp_skeleton.h"

IdleWarAction::IdleWarAction(Enemy* enemy): enemy(enemy) {
}

int IdleWarAction::execAction(float delta) {
	enemy->updateGravity(delta);
	enemy->rotateTowards(delta, enemy->getPlayerTransform()->getPosition(), enemy->rotationSpeed);
	if (enemy->idleWarTimer.elapsed() > enemy->getSkeleton()->getAnimationDuration(0)) {
		return Leave;
	}
	else {
		return Stay;
	}
}
