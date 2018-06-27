#include "mcv_platform.h"
#include "PropelledAction.h"

#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"

PropelledAction::PropelledAction(Enemy* enemy): enemy(enemy) {
}

int PropelledAction::execAction(float delta) {
	if (enemy->timer.elapsed() < enemy->propelDuration) {
		VEC3 deltaMovement = enemy->velocity * delta;
		enemy->updateGravity(delta);
		EnginePhysics.move(enemy->getCollider()->controller, toPxVec3(deltaMovement), delta);
		return Stay;
	}
	else {
		enemy->getSkeleton()->setTimeFactor(1);
		return Leave;
	}
}
