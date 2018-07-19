#include "mcv_platform.h"
#include "StunAction.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTACTION("StunAction", StunAction);

StunAction::StunAction(Enemy* enemy) :
	enemy(enemy) {
}

int StunAction::execAction(float delta) {
	if (enemy->stunTimer.elapsed() < enemy->stunDuration) {
		return Stay;
	}
	return Leave;
}

void StunAction::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}