#include "mcv_platform.h"
#include "IdleAction.h"
#include "components/ia/enemies/Enemy.h"
#include "skeleton/comp_skeleton.h"

IdleAction::IdleAction(Enemy* enemy, std::string animation) :
	enemy(enemy),
	animation(animation) {
}

int IdleAction::execAction(float delta) {
	enemy->updateGravity(delta);
	if (enemy->animationTimer.elapsed() < enemy->getSkeleton()->getAnimationDuration(animation)) {
		return Stay;
	}
	return Leave;
}
