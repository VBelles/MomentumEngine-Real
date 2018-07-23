#include "mcv_platform.h"
#include "PropelAction.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_hitboxes.h"

REGISTER_BTACTION("PropelAction", PropelAction);

PropelAction::PropelAction() {
	type = "PropelAction";
}

PropelAction::PropelAction(Enemy* enemy, std::string attack) :
	PropelAction() {
	this->enemy = enemy;
	this->attack = attack;
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

void PropelAction::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	attack = j.value("attack", attack);
}