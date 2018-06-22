#include "mcv_platform.h"
#include "OnGrabAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"
#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"

OnGrabAction::OnGrabAction(Dreidel* dreidel): dreidel(dreidel) {
}

int OnGrabAction::execAction(float delta) {
	dreidel->getSkeleton()->setTimeFactor(0);
	dreidel->getCollider()->destroy();
	dreidel->timer.reset();
	dreidel->grabbedDuration = dreidel->receivedAttack.grab->duration;
	return Leave;
}
