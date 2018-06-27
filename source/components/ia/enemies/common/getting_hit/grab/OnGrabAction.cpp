#include "mcv_platform.h"
#include "OnGrabAction.h"
#include "components/comp_collider.h"

OnGrabAction::OnGrabAction(Enemy* enemy): enemy(enemy) {
}

int OnGrabAction::execAction(float delta) {
	enemy->getCollider()->destroy(); //TODO enable/disable
	enemy->timer.reset();
	enemy->grabbedDuration = enemy->receivedAttack.grab->duration;
	return Leave;
}
