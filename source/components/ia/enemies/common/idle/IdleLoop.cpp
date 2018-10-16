#include "mcv_platform.h"
#include "IdleLoop.h"
#include "components/ia/enemies/Enemy.h"

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
	if (enemy->animationTimer.elapsed() >= enemy->getSkeleton()->getAnimationDuration(animation)) {
		return Leave;
	}
	else if (!cancelCondition.empty() && enemy->testCondition(cancelCondition, delta)) {
		enemy->getSkeleton()->clear(0.1f);
		return Leave;
	}
	else {
		return Stay;
	}
}

void IdleLoop::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
	cancelCondition = j.value("cancel_condition", cancelCondition);
}

void IdleLoop::debugInMenu() {
	ImGui::Text("Animation: %s\n", animation.c_str());
	ImGui::Text("Cancel condition: %s\n", cancelCondition.c_str());
}

void IdleLoop::onExit() {
	enemy->getSound()->stop("idle");
}
