#include "mcv_platform.h"
#include "AppearAction.h"
#include "components/ia/enemies/Enemy.h"
#include "skeleton/comp_skeleton.h"

AppearAction::AppearAction(Enemy* enemy, std::string animation) :
	enemy(enemy),
	animation(animation) {
}

int AppearAction::execAction(float delta) {
	enemy->updateGravity(delta);
	if (enemy->animationTimer.elapsed() < enemy->getSkeleton()->getAnimationDuration(animation)) {
		return Stay;
	}
	return Leave;
}
