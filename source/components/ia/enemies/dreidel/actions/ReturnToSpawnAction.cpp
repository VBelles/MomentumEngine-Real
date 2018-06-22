#include "mcv_platform.h"
#include "ReturnToSpawnAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"
#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"

ReturnToSpawnAction::ReturnToSpawnAction(Dreidel* dreidel): dreidel(dreidel) {
}

int ReturnToSpawnAction::execAction(float delta) {
	dreidel->getSkeleton()->blendCycle(1, 0.2f, 0.2f);
	dreidel->updateGravity(delta);

	dreidel->rotateTowards(delta, dreidel->spawnPosition, dreidel->rotationSpeed);
	TCompTransform* transform = dreidel->getTransform();
	//Move forward
	VEC3 myPosition = transform->getPosition();
	myPosition.y = 0;
	VEC3 targetPos = dreidel->spawnPosition;
	targetPos.y = 0;
	VEC3 myFront = transform->getFront();
	myFront.Normalize();
	VEC3 deltaMovement = myFront * dreidel->movementSpeed * delta;
	EnginePhysics.move(dreidel->getCollider()->controller, toPxVec3(deltaMovement), delta);

	float distanceSquared = VEC3::DistanceSquared(transform->getPosition(), dreidel->getPlayerTransform()->getPosition());
	if (VEC3::DistanceSquared(myPosition, targetPos) < dreidel->minCombatDistanceSqrd
		|| (distanceSquared < dreidel->fovChaseDistanceSqrd + dreidel->minCombatDistanceSqrd 
			&& transform->isInFov(dreidel->getPlayerTransform()->getPosition(), dreidel->attackFov))) {
		return Leave;
	}
	else {
		return Stay;
	}
}
