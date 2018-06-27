#include "mcv_platform.h"
#include "OnGrabAction.h"

#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"

OnGrabAction::OnGrabAction(Enemy* enemy): enemy(enemy) {
}

int OnGrabAction::execAction(float delta) {
	enemy->getSkeleton()->setTimeFactor(0);
	enemy->getCollider()->destroy();
	enemy->timer.reset();
	enemy->grabbedDuration = enemy->receivedAttack.grab->duration;
	return Leave;
}
