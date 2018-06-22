#include "mcv_platform.h"
#include "GrabbedAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"
#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"

GrabbedAction::GrabbedAction(Dreidel* dreidel): dreidel(dreidel) {
}

int GrabbedAction::execAction(float delta) {
	if (dreidel->timer.elapsed() >= dreidel->grabbedDuration) {
		dreidel->getCollider()->create();
		dreidel->getSkeleton()->setTimeFactor(1);
		return Leave;
	}
	return Stay;
}
