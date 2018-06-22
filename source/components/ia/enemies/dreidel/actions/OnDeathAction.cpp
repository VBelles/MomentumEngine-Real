#include "mcv_platform.h"
#include "OnDeathAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"
#include "components/comp_collider.h"
#include "components/comp_render.h"
#include "components/comp_respawner.h"

OnDeathAction::OnDeathAction(Dreidel* dreidel): dreidel(dreidel) {
}

int OnDeathAction::execAction(float delta) {
	dreidel->health = 0.f;
	dreidel->isDead = true;

	dreidel->getCollider()->destroy();

	TCompRender *render = dreidel->get<TCompRender>();
	render->disable();

	TCompRespawner* spawner = dreidel->get<TCompRespawner>();
	spawner->onDead();

	EngineScripting.throwEvent(onEnemyKilled, ((CEntity*)CHandle(dreidel).getOwner())->getName());

	return Leave;
}
