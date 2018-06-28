#include "mcv_platform.h"
#include "DeathAction.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"

DeathAction::DeathAction(Enemy* enemy, std::string animation) :
	enemy(enemy),
	animation(animation) {
}

int DeathAction::execAction(float delta) {
	if (enemy->animationTimer.elapsed() < enemy->getSkeleton()->getAnimationDuration(animation)) {
		return Stay;
	}
	return Leave;
}
