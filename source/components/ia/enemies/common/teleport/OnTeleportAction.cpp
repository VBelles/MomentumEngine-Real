#include "mcv_platform.h"
#include "OnTeleportAction.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTACTION("OnTeleportAction", OnTeleportAction);

OnTeleportAction::OnTeleportAction() {
	type = "OnTeleportAction";
}

OnTeleportAction::OnTeleportAction(Enemy* enemy, std::string animation) :
	OnTeleportAction() {
	this->enemy = enemy;
	this->animation = animation;
}

int OnTeleportAction::execAction(float delta) {
	enemy->getSkeleton()->executeAction(animation, 0.1f, 0.1f);
	enemy->animationTimer.reset();
	return Leave;
}

void OnTeleportAction::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
}

void OnTeleportAction::debugInMenu() {
	ImGui::Text("Animation: %s\n", animation.c_str());
}