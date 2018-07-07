#include "mcv_platform.h"
#include "IdleLoopFlying.h"
#include "components/ia/enemies/Enemy.h"
#include "skeleton/comp_skeleton.h"

IdleLoopFlying::IdleLoopFlying(Enemy* enemy, std::string animation, IBehaviorTreeCondition* cancelCondition) :
	enemy(enemy),
	animation(animation),
	cancelCondition(cancelCondition) {
}

int IdleLoopFlying::execAction(float delta) {
	if (enemy->animationTimer.elapsed() >= enemy->getSkeleton()->getAnimationDuration(animation)
		|| (cancelCondition && cancelCondition->testCondition(delta))) {
		return Leave;
	}
	else {
		return Stay;
	}
}
