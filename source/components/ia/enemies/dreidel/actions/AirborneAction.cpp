#include "mcv_platform.h"
#include "AirborneAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"
#include "skeleton/comp_skeleton.h"

AirborneAction::AirborneAction(Dreidel* dreidel): dreidel(dreidel) {
}

int AirborneAction::execAction(float delta) {
	dreidel->getSkeleton()->blendCycle(1, 0.2f, 0.2f);
	dreidel->updateGravity(delta);
	return Leave;
}
