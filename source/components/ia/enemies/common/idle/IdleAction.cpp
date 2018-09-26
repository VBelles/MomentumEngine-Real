#include "mcv_platform.h"
#include "IdleAction.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTACTION("IdleAction", IdleAction);

IdleAction::IdleAction() {
	type = "IdleAction";
}

IdleAction::IdleAction(Enemy* enemy, std::string animation, std::string cancelCondition) :
	IdleAction() {
	this->enemy = enemy;
	this->animation = animation;
	this->cancelCondition = cancelCondition;
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

void IdleAction::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
	cancelCondition = j.value("cancel_condition", cancelCondition);
}

void IdleAction::debugInMenu() {
	ImGui::Text("Animation: %s\n", animation.c_str());
	ImGui::Text("Cancel condition: %s\n", cancelCondition.c_str());
}
