#include "mcv_platform.h"
#include "OnStepBackAction.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_transform.h"

REGISTER_BTACTION("OnStepBackAction", OnStepBackAction);

OnStepBackAction::OnStepBackAction() {
	type = "OnStepBackAction";
}

OnStepBackAction::OnStepBackAction(Enemy* enemy, std::string animation) :
	OnStepBackAction() {
	this->enemy = enemy;
	this->animation = animation;
}

int OnStepBackAction::execAction(float delta) {
	VEC3 playerPosition = enemy->getPlayerTransform()->getPosition();
	enemy->rotateTowards(delta, playerPosition, enemy->rotationSpeed);
	enemy->getSkeleton()->executeAction(animation, 0.1f, 0.1f);
	enemy->animationTimer.reset();
	return Leave;
}

void OnStepBackAction::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
}

void OnStepBackAction::debugInMenu() {
	ImGui::Text("Animation: %s\n", animation.c_str());
}