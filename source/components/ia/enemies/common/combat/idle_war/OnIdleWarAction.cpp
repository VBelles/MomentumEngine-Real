#include "mcv_platform.h"
#include "OnIdleWarAction.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTACTION("OnIdleWarAction", OnIdleWarAction);

OnIdleWarAction::OnIdleWarAction() {
	type = "OnIdleWarAction";
}

OnIdleWarAction::OnIdleWarAction(Enemy* enemy, std::string animation) :
	OnIdleWarAction() {
	this->enemy = enemy;
	this->animation = animation;
}

int OnIdleWarAction::execAction(float delta) {
	VEC3 playerPosition = enemy->getPlayerTransform()->getPosition();
	enemy->rotateTowards(delta, playerPosition, enemy->rotationSpeed);
	enemy->getSkeleton()->blendCycle(animation, 0.1f, 0.1f);
	enemy->animationTimer.reset();
	enemy->getSound()->play("idle_war");
	return Leave;
}

void OnIdleWarAction::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
}

void OnIdleWarAction::debugInMenu() {
	ImGui::Text("Animation: %s\n", animation.c_str());
}
