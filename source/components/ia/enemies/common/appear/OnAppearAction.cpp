#include "mcv_platform.h"
#include "OnAppearAction.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTACTION("OnAppearAction", OnAppearAction);

OnAppearAction::OnAppearAction() {
	type = "OnAppearAction";
}

OnAppearAction::OnAppearAction(Enemy* enemy, std::string animation) :
	OnAppearAction() {
	this->enemy = enemy;
	this->animation = animation;
}

int OnAppearAction::execAction(float delta) {
	enemy->getSkeleton()->executeAction(animation, 0.f, 0.f);
	enemy->animationTimer.reset();
	enemy->getSound()->play("appear");
	return Leave;
}

void OnAppearAction::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
	animation = j.value("animation", animation);
}

void OnAppearAction::debugInMenu() {
	ImGui::Text("Animation: %s\n", animation.c_str());
}
