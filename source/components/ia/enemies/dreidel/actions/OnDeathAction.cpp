#include "mcv_platform.h"
#include "OnDeathAction.h"

#include "components/comp_collider.h"
#include "components/comp_render.h"
#include "components/comp_respawner.h"

OnDeathAction::OnDeathAction(Enemy* enemy): enemy(enemy) {
}

int OnDeathAction::execAction(float delta) {
	enemy->health = 0.f;
	enemy->isDead = true;

	enemy->getCollider()->destroy();

	TCompRender *render = enemy->get<TCompRender>();
	render->disable();

	TCompRespawner* spawner = enemy->get<TCompRespawner>();
	spawner->onDead();

	EngineScripting.throwEvent(onEnemyKilled, ((CEntity*)CHandle(enemy).getOwner())->getName());

	return Leave;
}
