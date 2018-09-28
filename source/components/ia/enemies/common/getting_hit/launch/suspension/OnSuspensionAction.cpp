#include "mcv_platform.h"
#include "OnSuspensionAction.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTACTION("OnSuspensionAction", OnSuspensionAction);

OnSuspensionAction::OnSuspensionAction() {
	type = "OnSuspensionAction";
}

OnSuspensionAction::OnSuspensionAction(Enemy* enemy, std::string animationStart, std::string animationLoop) :
	OnSuspensionAction() {
	this->enemy = enemy;
	this->animationStart = animationStart;
	this->animationLoop = animationLoop;
}

int OnSuspensionAction::execAction(float delta) {
	enemy->getSkeleton()->blendCycle(animationLoop, 0.1f, 0.1f);
	enemy->getSkeleton()->executeAction(animationStart, 0.1f, 0.1f);
	enemy->getSound()->play("launch_loop");
	return Leave;
}

void OnSuspensionAction::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animationStart = j.value("animation_start", animationStart);
	animationLoop = j.value("animation_loop", animationLoop);
}

void OnSuspensionAction::debugInMenu() {
	ImGui::Text("Animation start: %s\n", animationStart.c_str());
	ImGui::Text("Animation loop: %s\n", animationLoop.c_str());
}
