#include "mcv_platform.h"
#include "PropelAction.h"
#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"

PropelAction::PropelAction(Enemy* enemy): enemy(enemy) {
}

int PropelAction::execAction(float delta) {
	if (enemy->timer.elapsed() < enemy->propelDuration) {
		enemy->deltaMovement += enemy->velocity * delta;
		enemy->updateGravity(delta);
		return Stay;
	}
	else {
		return Leave;
	}
}
