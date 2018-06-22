#include "mcv_platform.h"
#include "IdleAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"
#include "skeleton/comp_skeleton.h"

IdleAction::IdleAction(Dreidel* dreidel): dreidel(dreidel) {
}

int IdleAction::execAction(float delta) {
	dreidel->getSkeleton()->blendCycle(0, 0.2f, 0.2f);
	dreidel->updateGravity(delta);
	return Leave;
}
