#include "mcv_platform.h"
#include "VerticalLaunchedAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"
#include "skeleton/comp_skeleton.h"

VerticalLaunchedAction::VerticalLaunchedAction(Dreidel* dreidel): dreidel(dreidel) {
}

int VerticalLaunchedAction::execAction(float delta) {
	dreidel->updateGravity(delta);
	if (dreidel->velocityVector.y <= 0) {
		dreidel->timer.reset();
		dreidel->getSkeleton()->setTimeFactor(1);
		return Leave;
	}
	else {
		return Stay;
	}
}
