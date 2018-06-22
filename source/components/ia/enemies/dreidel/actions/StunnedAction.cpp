#include "mcv_platform.h"
#include "StunnedAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"
#include "skeleton/comp_skeleton.h"

StunnedAction::StunnedAction(Dreidel* dreidel): dreidel(dreidel) {
}

int StunnedAction::execAction(float delta) {
	dreidel->updateGravity(delta);
	if (dreidel->stunTimer.elapsed() > dreidel->stunDuration) {
		dreidel->isStunned = false;
		dreidel->getSkeleton()->setTimeFactor(1);
		return Leave;
	}
	else {
		return Stay;
	}
}
