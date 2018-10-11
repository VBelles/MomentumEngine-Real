#include "mcv_platform.h"
#include "AppearAction.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTACTION("AppearAction", AppearAction);

AppearAction::AppearAction() {
	type = "AppearAction";
}

AppearAction::AppearAction(Enemy* enemy, std::string animation) :
	AppearAction() {
	this->enemy = enemy;
	this->animation = animation;
}

int AppearAction::execAction(float delta) {
	if (enemy->animationTimer.elapsed() < enemy->getSkeleton()->getAnimationDuration(animation) - animationEndPadding) {
		return Stay;
	}
	return Leave;
}

void AppearAction::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
	animationEndPadding = j.value("enimation_end_padding", animationEndPadding);
}

void AppearAction::debugInMenu() {
	ImGui::Text("Animation: %s\n", animation.c_str());
	ImGui::DragFloat("Animation end padding", &animationEndPadding, 0.01f, 0.f, 50.f);
}
