#include "mcv_platform.h"
#include "IdleLoopFlying.h"
#include "components/ia/enemies/Enemy.h"
#include "skeleton/comp_skeleton.h"

REGISTER_BTACTION("IdleLoopFlying", IdleLoopFlying);

IdleLoopFlying::IdleLoopFlying() {
	type = "IdleLoopFlying";
}

IdleLoopFlying::IdleLoopFlying(Enemy* enemy, std::string animation, std::string cancelCondition) :
	IdleLoopFlying() {
	this->enemy = enemy;
	this->animation = animation;
	this->cancelCondition = cancelCondition;
}

int IdleLoopFlying::execAction(float delta) {
	if (enemy->animationTimer.elapsed() >= enemy->getSkeleton()->getAnimationDuration(animation)
		|| (!cancelCondition.empty() && enemy->testCondition(cancelCondition, delta))) {
		return Leave;
	}
	else {
		return Stay;
	}
}

void IdleLoopFlying::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
	cancelCondition = j.value("cancel_condition", cancelCondition);
}

void IdleLoopFlying::debugInMenu() {
	ImGui::Text("Animation: %s\n", animation.c_str());
	ImGui::Text("Cancel condition: %s\n", cancelCondition.c_str());
}