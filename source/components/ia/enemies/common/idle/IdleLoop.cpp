#include "mcv_platform.h"
#include "IdleLoop.h"
#include "components/ia/enemies/Enemy.h"
#include "skeleton/comp_skeleton.h"

REGISTER_BTACTION("IdleLoop", IdleLoop);

IdleLoop::IdleLoop() {
	type = "IdleLoop";
}

IdleLoop::IdleLoop(Enemy* enemy, std::string animation, std::string cancelCondition) :
	IdleLoop() {
	this->enemy = enemy;
	this->animation = animation;
	this->cancelCondition = cancelCondition;
}

int IdleLoop::execAction(float delta) {
	enemy->updateGravity(delta);
	if (enemy->animationTimer.elapsed() >= enemy->getSkeleton()->getAnimationDuration(animation)
		|| (!cancelCondition.empty() && enemy->testCondition(cancelCondition, delta))) {
		return Leave;
	}
	else {
		return Stay;
	}
}

void IdleLoop::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
	cancelCondition = j.value("cancel_condition", cancelCondition);
}