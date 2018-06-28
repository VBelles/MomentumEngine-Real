#include "mcv_platform.h"
#include "AirborneAction.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"

AirborneAction::AirborneAction(Enemy* enemy): enemy(enemy) {
}

int AirborneAction::execAction(float delta) {
	enemy->getSkeleton()->blendCycle(1, 0.2f, 0.2f);
	enemy->updateGravity(delta);
	return Leave;
}
