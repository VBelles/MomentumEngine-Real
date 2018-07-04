#include "mcv_platform.h"
#include "IdleLoopFlying.h"
#include "components/ia/enemies/Enemy.h"
#include "skeleton/comp_skeleton.h"

IdleLoopFlying::IdleLoopFlying(Enemy* enemy, std::string animation) :
	enemy(enemy),
	animation(animation) {
}

int IdleLoopFlying::execAction(float delta) {
	if (enemy->animationTimer.elapsed() < enemy->getSkeleton()->getAnimationDuration(animation)) {
		return Stay;
	}
	return Leave;
}
