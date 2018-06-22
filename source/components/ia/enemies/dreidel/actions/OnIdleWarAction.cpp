#include "mcv_platform.h"
#include "OnIdleWarAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"
#include "skeleton/comp_skeleton.h"
#include "components/comp_transform.h"

OnIdleWarAction::OnIdleWarAction(Dreidel* dreidel): dreidel(dreidel) {
}

int OnIdleWarAction::execAction(float delta) {
	dreidel->getSkeleton()->blendCycle(0, 0.2f, 0.2f);
	dreidel->updateGravity(delta);
	dreidel->rotateTowards(delta, dreidel->getPlayerTransform()->getPosition(), dreidel->rotationSpeed);
	dreidel->idleWarTimer.reset();
	return Leave;
}
