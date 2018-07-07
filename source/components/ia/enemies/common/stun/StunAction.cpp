#include "mcv_platform.h"
#include "StunAction.h"
#include "components/ia/enemies/Enemy.h"

StunAction::StunAction(Enemy* enemy) :
	enemy(enemy) {
}

int StunAction::execAction(float delta) {
	if (enemy->stunTimer.elapsed() < enemy->stunDuration) {
		return Stay;
	}
	return Leave;
}
