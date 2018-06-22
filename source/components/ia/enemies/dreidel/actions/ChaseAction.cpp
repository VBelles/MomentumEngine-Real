#include "mcv_platform.h"
#include "ChaseAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"
#include "skeleton/comp_skeleton.h"
#include "components/comp_transform.h"
#include "components/comp_collider.h"

ChaseAction::ChaseAction(Dreidel* dreidel): dreidel(dreidel) {
}

int ChaseAction::execAction(float delta) {
	dreidel->getSkeleton()->blendCycle(1, 0.2f, 0.2f);
	dreidel->updateGravity(delta);

	dreidel->rotateTowards(delta, dreidel->getPlayerTransform()->getPosition(), dreidel->rotationSpeed);

	//Move forward
	VEC3 myPosition = dreidel->getTransform()->getPosition();
	VEC3 myFront = dreidel->getTransform()->getFront();
	myFront.Normalize();
	VEC3 deltaMovement = myFront * dreidel->movementSpeed * delta;
	EnginePhysics.move(dreidel->getCollider()->controller, toPxVec3(deltaMovement), delta);
	return Leave;
}
