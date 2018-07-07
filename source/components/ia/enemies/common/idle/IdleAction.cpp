#include "mcv_platform.h"
#include "IdleAction.h"
#include "components/ia/enemies/Enemy.h"
#include "skeleton/comp_skeleton.h"

IdleAction::IdleAction(Enemy* enemy, std::string animation, IBehaviorTreeCondition* cancelCondition) :
	enemy(enemy),
	animation(animation),
	cancelCondition(cancelCondition) {
}

int IdleAction::execAction(float delta) {
	enemy->updateGravity(delta);
	if (enemy->animationTimer.elapsed() >= enemy->getSkeleton()->getAnimationDuration(animation)
		|| (cancelCondition && cancelCondition->testCondition(delta))) {
		return Leave;
	}
	else {
		return Stay;
	}
}
