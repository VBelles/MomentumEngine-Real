#include "mcv_platform.h"
#include "HorizontalLaunchedAction.h"

#include "components/comp_collider.h"
#include "components/comp_transform.h"
#include "skeleton/comp_skeleton.h"

HorizontalLaunchedAction::HorizontalLaunchedAction(Enemy* enemy): enemy(enemy) {
}

int HorizontalLaunchedAction::execAction(float delta) {
	enemy->updateGravity(delta);
	VEC3 deltaMovement = enemy->velocity * delta;
	EnginePhysics.move(enemy->getCollider()->controller, PxVec3(deltaMovement.x, 0, deltaMovement.z), delta);
	if (enemy->getTransform()->getPosition().y + deltaMovement.y - enemy->initialLaunchPos.y <= 0.001 || enemy->grounded) {
		enemy->velocity.x = 0;
		enemy->velocity.z = 0;
		enemy->timer.reset();
		enemy->getSkeleton()->setTimeFactor(1);
		return Leave;
	}
	else {
		return Stay;
	}
}
