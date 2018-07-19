#include "mcv_platform.h"
#include "IdleAction.h"
#include "components/ia/enemies/Enemy.h"
#include "skeleton/comp_skeleton.h"

REGISTER_BTACTION("IdleAction", IdleAction);

IdleAction::IdleAction(Enemy* enemy, std::string animation, std::string cancelCondition) :
	enemy(enemy),
	animation(animation),
	cancelCondition(cancelCondition) {
}

int IdleAction::execAction(float delta) {
	enemy->updateGravity(delta);
	if (enemy->animationTimer.elapsed() >= enemy->getSkeleton()->getAnimationDuration(animation)
		|| (!cancelCondition.empty() && enemy->testCondition(cancelCondition, delta))) {
		return Leave;
	}
	else {
		return Stay;
	}
}

void IdleAction::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
	cancelCondition = j.value("cancel_condition", cancelCondition);
}