#include "mcv_platform.h"
#include "RespawnAction.h"

#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "components/comp_render.h"
#include "components/comp_give_power.h"
#include "skeleton/comp_skeleton.h"

RespawnAction::RespawnAction(Enemy* enemy): enemy(enemy) {
}

int RespawnAction::execAction(float delta) {
	enemy->health = enemy->maxHealth;
	enemy->isDead = false;

	enemy->getCollider()->create();
	enemy->getTransform()->setPosition(enemy->spawnPosition);
	enemy->getCollider()->controller->setFootPosition(toPxtendedVec3(enemy->spawnPosition));

	TCompRender *render = enemy->get<TCompRender>();
	render->enable();

	TCompGivePower *power = enemy->get<TCompGivePower>();
	power->reset();

	enemy->getSkeleton()->blendCycle(0);
	enemy->getSkeleton()->setTimeFactor(1);

	return Leave;
}
