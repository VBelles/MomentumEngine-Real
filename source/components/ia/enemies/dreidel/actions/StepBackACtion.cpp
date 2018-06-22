#include "mcv_platform.h"
#include "StepBackAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"
#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"

StepBackAction::StepBackAction(Dreidel* dreidel): dreidel(dreidel) {
}

int StepBackAction::execAction(float delta) {
	dreidel->getSkeleton()->blendCycle(1, 0.2f, 0.2f);
	dreidel->updateGravity(delta);

	dreidel->rotateTowards(delta, dreidel->getPlayerTransform()->getPosition(), dreidel->rotationSpeed);

	VEC3 myPosition = dreidel->getTransform()->getPosition();
	VEC3 myFront = dreidel->getTransform()->getFront();
	myFront.Normalize();
	VEC3 deltaMovement = -myFront * dreidel->stepBackSpeed * delta;
	EnginePhysics.move(dreidel->getCollider()->controller, toPxVec3(deltaMovement), delta);
	return Leave;
}
