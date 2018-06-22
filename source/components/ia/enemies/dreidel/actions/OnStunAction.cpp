#include "mcv_platform.h"
#include "OnStunAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"
#include "skeleton/comp_skeleton.h"

OnStunAction::OnStunAction(Dreidel* dreidel): dreidel(dreidel) {
}

int OnStunAction::execAction(float delta) {
	dreidel->getSkeleton()->setTimeFactor(0);
	dreidel->stunDuration = dreidel->receivedAttack.stun->duration;
	dreidel->stunTimer.reset();
	dreidel->isStunned = true;
	return Leave;
}
