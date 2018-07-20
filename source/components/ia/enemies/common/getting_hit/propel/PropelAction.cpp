#include "mcv_platform.h"
#include "PropelAction.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTACTION("PropelAction", PropelAction);

PropelAction::PropelAction() {
	type = "PropelAction";
}

PropelAction::PropelAction(Enemy* enemy) :
	PropelAction() {
	this->enemy = enemy;
}


int PropelAction::execAction(float delta) {
	if (enemy->timer.elapsed() < enemy->propelDuration) {
		enemy->deltaMovement += enemy->velocity * delta;
		enemy->updateGravity(delta);
		return Stay;
	}
	else {
		return Leave;
	}
}

void PropelAction::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}