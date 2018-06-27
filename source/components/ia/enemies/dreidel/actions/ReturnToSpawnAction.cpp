#include "mcv_platform.h"
#include "ReturnToSpawnAction.h"

#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"

ReturnToSpawnAction::ReturnToSpawnAction(Enemy* enemy): enemy(enemy) {
}

int ReturnToSpawnAction::execAction(float delta) {
	enemy->getSkeleton()->blendCycle(1, 0.2f, 0.2f);
	enemy->updateGravity(delta);

	enemy->rotateTowards(delta, enemy->spawnPosition, enemy->rotationSpeed);
	TCompTransform* transform = enemy->getTransform();
	//Move forward
	VEC3 myPosition = transform->getPosition();
	myPosition.y = 0;
	VEC3 targetPos = enemy->spawnPosition;
	targetPos.y = 0;
	VEC3 myFront = transform->getFront();
	myFront.Normalize();
	VEC3 deltaMovement = myFront * enemy->movementSpeed * delta;
	EnginePhysics.move(enemy->getCollider()->controller, toPxVec3(deltaMovement), delta);

	float distanceSquared = VEC3::DistanceSquared(transform->getPosition(), enemy->getPlayerTransform()->getPosition());
	if (VEC3::DistanceSquared(myPosition, targetPos) < enemy->minCombatDistanceSqrd
		|| (distanceSquared < enemy->fovChaseDistanceSqrd + enemy->minCombatDistanceSqrd 
			&& transform->isInFov(enemy->getPlayerTransform()->getPosition(), enemy->attackFov))) {
		return Leave;
	}
	else {
		return Stay;
	}
}
