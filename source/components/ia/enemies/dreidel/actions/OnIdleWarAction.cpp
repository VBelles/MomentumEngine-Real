#include "mcv_platform.h"
#include "OnIdleWarAction.h"

#include "skeleton/comp_skeleton.h"
#include "components/comp_transform.h"

OnIdleWarAction::OnIdleWarAction(Enemy* enemy): enemy(enemy) {
}

int OnIdleWarAction::execAction(float delta) {
	enemy->getSkeleton()->blendCycle(0, 0.2f, 0.2f);
	enemy->updateGravity(delta);
	enemy->rotateTowards(delta, enemy->getPlayerTransform()->getPosition(), enemy->rotationSpeed);
	enemy->idleWarTimer.reset();
	return Leave;
}
