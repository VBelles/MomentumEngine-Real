#include "mcv_platform.h"
#include "OnHorizontalLaunchAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"
#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"

OnHorizontalLaunchAction::OnHorizontalLaunchAction(Dreidel* dreidel): dreidel(dreidel) {
}

int OnHorizontalLaunchAction::execAction(float delta) {
	dreidel->getSkeleton()->setTimeFactor(0);
	dreidel->floatingDuration = dreidel->receivedAttack.horizontalLauncher->suspensionDuration;
	dreidel->initialLaunchPos = dreidel->getTransform()->getPosition();
	dreidel->velocityVector = dreidel->receivedAttack.horizontalLauncher->velocity;
	return Leave;
}
