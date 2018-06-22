#include "mcv_platform.h"
#include "OnVerticalLaunchAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"
#include "skeleton/comp_skeleton.h"

OnVerticalLaunchAction::OnVerticalLaunchAction(Dreidel* dreidel): dreidel(dreidel) {
}

int OnVerticalLaunchAction::execAction(float delta) {
	dreidel->getSkeleton()->setTimeFactor(0);
	dreidel->floatingDuration = dreidel->receivedAttack.verticalLauncher->suspensionDuration;
	dreidel->velocityVector = dreidel->receivedAttack.verticalLauncher->velocity;
	return Leave;
}
