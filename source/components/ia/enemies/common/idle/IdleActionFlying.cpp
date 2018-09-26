#include "mcv_platform.h"
#include "IdleActionFlying.h"
#include "components/ia/enemies/Enemy.h"
#include "skeleton/comp_skeleton.h"

REGISTER_BTACTION("IdleActionFlying", IdleActionFlying);

IdleActionFlying::IdleActionFlying() {
	type = "IdleActionFlying";
}

IdleActionFlying::IdleActionFlying(Enemy* enemy, std::string animation, std::string cancelCondition) :
	IdleActionFlying() {
	this->enemy = enemy;
	this->animation = animation;
	this->cancelCondition = cancelCondition;
}

int IdleActionFlying::execAction(float delta) {
	if (enemy->animationTimer.elapsed() >= enemy->getSkeleton()->getAnimationDuration(animation)
		|| (!cancelCondition.empty() && enemy->testCondition(cancelCondition, delta))) {
		return Leave;
	}
	else {
		return Stay;
	}
}

void IdleActionFlying::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
	cancelCondition = j.value("cancel_condition", cancelCondition);
}

void IdleActionFlying::debugInMenu() {
	ImGui::Text("Animation: %s\n", animation.c_str());
	ImGui::Text("Cancel condition: %s\n", cancelCondition.c_str());
}