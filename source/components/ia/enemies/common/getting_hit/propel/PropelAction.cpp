#include "mcv_platform.h"
#include "PropelAction.h"
#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_hitboxes.h"

PropelAction::PropelAction(Enemy* enemy, std::string attack): 
	enemy(enemy),
	attack(attack) {
}

int PropelAction::execAction(float delta) {
	if (enemy->timer.elapsed() < enemy->propelDuration) {
		enemy->deltaMovement += enemy->velocity * delta;
		enemy->updateGravity(delta);
		enemy->getHitboxes()->disable(enemy->attacks[attack].hitboxName);
		enemy->currentAttack = "";
		return Stay;
	}
	else {
		return Leave;
	}
}
