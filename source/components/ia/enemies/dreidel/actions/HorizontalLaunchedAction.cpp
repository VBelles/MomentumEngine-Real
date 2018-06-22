#include "mcv_platform.h"
#include "HorizontalLaunchedAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"
#include "components/comp_collider.h"
#include "components/comp_transform.h"
#include "skeleton/comp_skeleton.h"

HorizontalLaunchedAction::HorizontalLaunchedAction(Dreidel* dreidel): dreidel(dreidel) {
}

int HorizontalLaunchedAction::execAction(float delta) {
	dreidel->updateGravity(delta);
	VEC3 deltaMovement = dreidel->velocityVector * delta;
	EnginePhysics.move(dreidel->getCollider()->controller, PxVec3(deltaMovement.x, 0, deltaMovement.z), delta);
	if (dreidel->getTransform()->getPosition().y + deltaMovement.y - dreidel->initialLaunchPos.y <= 0.001 || dreidel->grounded) {
		dreidel->velocityVector.x = 0;
		dreidel->velocityVector.z = 0;
		dreidel->timer.reset();
		dreidel->getSkeleton()->setTimeFactor(1);
		return Leave;
	}
	else {
		return Stay;
	}
}
