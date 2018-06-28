#include "mcv_platform.h"
#include "BlockingBreakAction.h"
#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"

BlockingBreakAction::BlockingBreakAction(Enemy* enemy, std::string animation) :
	enemy(enemy),
	animation(animation) {
}

int BlockingBreakAction::execAction(float delta) {
	if (enemy->animationTimer.elapsed() < enemy->getSkeleton()->getAnimationDuration(animation)) {
		return Stay;
	}
	return Leave;
}
