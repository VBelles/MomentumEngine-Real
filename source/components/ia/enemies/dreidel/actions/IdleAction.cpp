#include "mcv_platform.h"
#include "IdleAction.h"

#include "skeleton/comp_skeleton.h"

IdleAction::IdleAction(Enemy* enemy): enemy(enemy) {
}

int IdleAction::execAction(float delta) {
	enemy->getSkeleton()->blendCycle(0, 0.2f, 0.2f);
	enemy->updateGravity(delta);
	return Leave;
}
