#include "mcv_platform.h"
#include "StepBackAction.h"

#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"

StepBackAction::StepBackAction(Enemy* enemy): enemy(enemy) {
}

int StepBackAction::execAction(float delta) {
	enemy->getSkeleton()->blendCycle(1, 0.2f, 0.2f);
	enemy->updateGravity(delta);

	enemy->rotateTowards(delta, enemy->getPlayerTransform()->getPosition(), enemy->rotationSpeed);

	VEC3 myPosition = enemy->getTransform()->getPosition();
	VEC3 myFront = enemy->getTransform()->getFront();
	myFront.Normalize();
	VEC3 deltaMovement = -myFront * enemy->stepBackSpeed * delta;
	EnginePhysics.move(enemy->getCollider()->controller, toPxVec3(deltaMovement), delta);
	return Leave;
}
