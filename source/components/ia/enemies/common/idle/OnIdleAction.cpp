#include "mcv_platform.h"
#include "OnIdleAction.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTACTION("OnIdleAction", OnIdleAction);

OnIdleAction::OnIdleAction() {
	type = "OnIdleAction";
}

OnIdleAction::OnIdleAction(Enemy* enemy, std::string animation) :
	OnIdleAction() {
	this->enemy = enemy;
	this->animation = animation;
}

int OnIdleAction::execAction(float delta) {
	enemy->getSkeleton()->executeAction(animation, 0.1f, 0.1f);
	enemy->animationTimer.reset();
	return Leave;
}

void OnIdleAction::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
}

void OnIdleAction::debugInMenu() {
	ImGui::Text("Animation: %s\n", animation.c_str());
}