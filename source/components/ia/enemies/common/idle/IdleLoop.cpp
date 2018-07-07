#include "mcv_platform.h"
#include "IdleLoop.h"
#include "components/ia/enemies/Enemy.h"
#include "skeleton/comp_skeleton.h"

IdleLoop::IdleLoop(Enemy* enemy, std::string animation, IBehaviorTreeCondition* cancelCondition) :
	enemy(enemy),
	animation(animation),
	cancelCondition(cancelCondition) {
}

int IdleLoop::execAction(float delta) {
	enemy->updateGravity(delta);
	if (enemy->animationTimer.elapsed() >= enemy->getSkeleton()->getAnimationDuration(animation)
		|| (cancelCondition && cancelCondition->testCondition(delta))) {
		return Leave;
	}
	else {
		return Stay;
	}
}
