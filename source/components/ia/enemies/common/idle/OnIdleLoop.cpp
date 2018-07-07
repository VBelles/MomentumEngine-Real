#include "mcv_platform.h"
#include "OnIdleLoop.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"

OnIdleLoop::OnIdleLoop(Enemy* enemy, std::string animation):
	enemy(enemy),
	animation(animation){
}

int OnIdleLoop::execAction(float delta) {
	enemy->getSkeleton()->blendCycle(animation, 0.1f, 0.1f);
	enemy->animationTimer.reset();
	return Leave;
}
