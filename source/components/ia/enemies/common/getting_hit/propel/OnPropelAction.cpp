#include "mcv_platform.h"
#include "OnPropelAction.h"
#include "components/comp_collider.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_hitboxes.h"

OnPropelAction::OnPropelAction(Enemy* enemy, std::string attack): 
	enemy(enemy), 
	attack(attack) {
}

int OnPropelAction::execAction(float delta) {
	enemy->getCollider()->create(); //TODO enable/disable
	enemy->velocity = enemy->receivedAttack.propel->velocity;
	enemy->propelDuration = enemy->receivedAttack.propel->duration;
	enemy->timer.reset();
	enemy->currentAttack = attack;
	enemy->getHitboxes()->enable(enemy->attacks[attack].hitboxName);
	enemy->isBlocking = false;
	enemy->blockingBroken = false;
	return Leave;
}
