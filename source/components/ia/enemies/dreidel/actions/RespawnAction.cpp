#include "mcv_platform.h"
#include "RespawnAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"
#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "components/comp_render.h"
#include "components/comp_give_power.h"
#include "skeleton/comp_skeleton.h"

RespawnAction::RespawnAction(Dreidel* dreidel): dreidel(dreidel) {
}

int RespawnAction::execAction(float delta) {
	dreidel->health = dreidel->maxHealth;
	dreidel->isDead = false;

	dreidel->getCollider()->create();
	dreidel->getTransform()->setPosition(dreidel->spawnPosition);
	dreidel->getCollider()->controller->setFootPosition(toPxtendedVec3(dreidel->spawnPosition));

	TCompRender *render = dreidel->get<TCompRender>();
	render->enable();

	TCompGivePower *power = dreidel->get<TCompGivePower>();
	power->reset();

	dreidel->getSkeleton()->blendCycle(0);
	dreidel->getSkeleton()->setTimeFactor(1);

	return Leave;
}
