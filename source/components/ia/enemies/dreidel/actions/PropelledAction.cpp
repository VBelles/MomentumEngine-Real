#include "mcv_platform.h"
#include "PropelledAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"
#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"

PropelledAction::PropelledAction(Dreidel* dreidel): dreidel(dreidel) {
}

int PropelledAction::execAction(float delta) {
	if (dreidel->timer.elapsed() < dreidel->propelDuration) {
		VEC3 deltaMovement = dreidel->velocityVector * delta;
		dreidel->updateGravity(delta);
		EnginePhysics.move(dreidel->getCollider()->controller, toPxVec3(deltaMovement), delta);
		return Stay;
	}
	else {
		dreidel->getSkeleton()->setTimeFactor(1);
		return Leave;
	}
}
