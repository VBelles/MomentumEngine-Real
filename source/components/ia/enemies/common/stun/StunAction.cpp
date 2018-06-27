#include "mcv_platform.h"
#include "StunAction.h"

StunAction::StunAction(Enemy* enemy) :
	enemy(enemy) {
}

int StunAction::execAction(float delta) {
	if (enemy->stunTimer.elapsed() < enemy->stunDuration) {
		return Stay;
	}
	return Leave;
}
