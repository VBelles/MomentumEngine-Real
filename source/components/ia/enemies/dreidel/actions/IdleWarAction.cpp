#include "mcv_platform.h"
#include "IdleWarAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"
#include "components/comp_transform.h"
#include "skeleton/comp_skeleton.h"

IdleWarAction::IdleWarAction(Dreidel* dreidel): dreidel(dreidel) {
}

int IdleWarAction::execAction(float delta) {
	dreidel->updateGravity(delta);
	dreidel->rotateTowards(delta, dreidel->getPlayerTransform()->getPosition(), dreidel->rotationSpeed);
	if (dreidel->idleWarTimer.elapsed() > dreidel->getSkeleton()->getAnimationDuration(0)) {
		return Leave;
	}
	else {
		return Stay;
	}
}
