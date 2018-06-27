#include "mcv_platform.h"
#include "OnPropelAction.h"

#include "components/comp_collider.h"
#include "components/comp_render.h"
#include "skeleton/comp_skeleton.h"

OnPropelAction::OnPropelAction(Enemy* enemy): enemy(enemy) {
}

int OnPropelAction::execAction(float delta) {
	enemy->getCollider()->create();
	enemy->velocity = enemy->receivedAttack.propel->velocity;
	if (enemy->receivedAttack.propel->duration > 0.f) {
		enemy->propelDuration = enemy->receivedAttack.propel->duration;
	}
	else {
		enemy->propelDuration = enemy->defaultPropelDuration;
	}
	enemy->getSkeleton()->setTimeFactor(0);

	enemy->timer.reset();

	TCompRender* render = enemy->get<TCompRender>();
	render->TurnRed(0.5f);
	return Leave;
}
