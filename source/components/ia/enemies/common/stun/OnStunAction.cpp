#include <algorithm>
#include "mcv_platform.h"
#include "OnStunAction.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTACTION("OnStunAction", OnStunAction);

OnStunAction::OnStunAction() {
	type = "OnStunAction";
}

OnStunAction::OnStunAction(Enemy* enemy, std::string animationStart, std::string animationLoop) :
	OnStunAction() {
	this->enemy = enemy;
	this->animationStart = animationStart;
	this->animationLoop = animationLoop;
}

int OnStunAction::execAction(float delta) {
	enemy->stunDuration = std::max(enemy->stunDuration, enemy->getSkeleton()->getAnimationDuration(animationStart));
	enemy->getSkeleton()->blendCycle(animationLoop, 0.1f, 0.1f);
	enemy->getSkeleton()->executeAction(animationStart, 0.1f, 0.1f);
	return Leave;
}

void OnStunAction::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animationStart = j.value("animation_start", animationStart);
	animationLoop = j.value("animation_loop", animationLoop);
}

void OnStunAction::debugInMenu() {
	ImGui::Text("Animation start: %s\n", animationStart.c_str());
	ImGui::Text("Animation loop: %s\n", animationLoop.c_str());
}
