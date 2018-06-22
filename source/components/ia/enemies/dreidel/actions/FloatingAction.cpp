#include "mcv_platform.h"
#include "FloatingAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"
#include "skeleton/comp_skeleton.h"

FloatingAction::FloatingAction(Dreidel* dreidel): dreidel(dreidel) {
}

int FloatingAction::execAction(float delta) {
	dreidel->getSkeleton()->setTimeFactor(0);
	if (dreidel->timer.elapsed() > dreidel->floatingDuration) {
		dreidel->getSkeleton()->setTimeFactor(1);
		return Leave;
	}
	else {
		return Stay;
	}
}
