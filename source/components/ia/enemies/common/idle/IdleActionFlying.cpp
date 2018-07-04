#include "mcv_platform.h"
#include "IdleActionFlying.h"
#include "components/ia/enemies/Enemy.h"
#include "skeleton/comp_skeleton.h"

IdleActionFlying::IdleActionFlying(Enemy* enemy, std::string animation) :
	enemy(enemy),
	animation(animation) {
}

int IdleActionFlying::execAction(float delta) {
	if (enemy->animationTimer.elapsed() < enemy->getSkeleton()->getAnimationDuration(animation)) {
		return Stay;
	}
	return Leave;
}
