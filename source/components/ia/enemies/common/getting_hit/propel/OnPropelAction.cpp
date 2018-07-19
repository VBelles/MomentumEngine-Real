#include "mcv_platform.h"
#include "OnPropelAction.h"
#include "components/comp_collider.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTACTION("OnPropelAction", OnPropelAction);

OnPropelAction::OnPropelAction(Enemy* enemy): enemy(enemy) {
}

int OnPropelAction::execAction(float delta) {
	enemy->getCollider()->create(); //TODO enable/disable
	enemy->velocity = enemy->receivedAttack.propel->velocity;
	enemy->propelDuration = enemy->receivedAttack.propel->duration;
	enemy->timer.reset();
	enemy->isBlocking = false;
	enemy->blockingBroken = false;
	return Leave;
}

void OnPropelAction::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}