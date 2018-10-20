#include "mcv_platform.h"
#include "OnChaseAction.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTACTION("OnChaseAction", OnChaseAction);

OnChaseAction::OnChaseAction() {
	type = "OnChaseAction";
}

OnChaseAction::OnChaseAction(Enemy* enemy, std::string animation) :
	OnChaseAction() {
	this->enemy = enemy;
	this->animation = animation;
}

int OnChaseAction::execAction(float delta) {
	VEC3 playerPosition = enemy->getPlayerTransform()->getPosition();
	enemy->rotateTowards(delta, playerPosition, enemy->rotationSpeed);
	enemy->getSkeleton()->blendCycle(animation, 0.1f, 0.1f);
	enemy->getSound()->play("chase");
	return Leave;
}

void OnChaseAction::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
}

void OnChaseAction::debugInMenu() {
	ImGui::Text("Animation: %s\n", animation.c_str());
}
