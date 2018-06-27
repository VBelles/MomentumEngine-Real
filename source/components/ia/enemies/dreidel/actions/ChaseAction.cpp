#include "mcv_platform.h"
#include "ChaseAction.h"

#include "skeleton/comp_skeleton.h"
#include "components/comp_transform.h"
#include "components/comp_collider.h"

ChaseAction::ChaseAction(Enemy* enemy): enemy(enemy) {
}

int ChaseAction::execAction(float delta) {
	enemy->getSkeleton()->blendCycle(1, 0.2f, 0.2f);
	enemy->updateGravity(delta);

	enemy->rotateTowards(delta, enemy->getPlayerTransform()->getPosition(), enemy->rotationSpeed);

	//Move forward
	VEC3 myPosition = enemy->getTransform()->getPosition();
	VEC3 myFront = enemy->getTransform()->getFront();
	myFront.Normalize();
	VEC3 deltaMovement = myFront * enemy->movementSpeed * delta;
	EnginePhysics.move(enemy->getCollider()->controller, toPxVec3(deltaMovement), delta);
	return Leave;
}
