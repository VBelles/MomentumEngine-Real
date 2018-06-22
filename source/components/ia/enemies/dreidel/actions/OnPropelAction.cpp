#include "mcv_platform.h"
#include "OnPropelAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"
#include "components/comp_collider.h"
#include "components/comp_render.h"
#include "skeleton/comp_skeleton.h"

OnPropelAction::OnPropelAction(Dreidel* dreidel): dreidel(dreidel) {
}

int OnPropelAction::execAction(float delta) {
	dreidel->getCollider()->create();
	dreidel->velocityVector = dreidel->receivedAttack.propel->velocity;
	if (dreidel->receivedAttack.propel->duration > 0.f) {
		dreidel->propelDuration = dreidel->receivedAttack.propel->duration;
	}
	else {
		dreidel->propelDuration = dreidel->defaultPropelDuration;
	}
	dreidel->getSkeleton()->setTimeFactor(0);

	dreidel->timer.reset();

	TCompRender* render = dreidel->get<TCompRender>();
	render->TurnRed(0.5f);
	return Leave;
}
