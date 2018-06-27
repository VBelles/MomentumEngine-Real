#include "mcv_platform.h"
#include "HitStun.h"
#include "skeleton/comp_skeleton.h"

HitStun::HitStun(Enemy* enemy, std::string animation) :
	enemy(enemy),
	animation(animation) {
}

int HitStun::execAction(float delta) {
	if (enemy->animationTimer.elapsed() < enemy->getSkeleton()->getAnimationDuration(animation)) {
		return Stay;
	}
	return Leave;
}
