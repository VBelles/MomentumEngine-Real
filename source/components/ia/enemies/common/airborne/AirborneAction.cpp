#include "mcv_platform.h"
#include "AirborneAction.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTAction("AirborneAction", AirborneAction);

AirborneAction::AirborneAction(Enemy* enemy): enemy(enemy) {
}

int AirborneAction::execAction(float delta) {
	enemy->updateGravity(delta);
	return Leave;
}
