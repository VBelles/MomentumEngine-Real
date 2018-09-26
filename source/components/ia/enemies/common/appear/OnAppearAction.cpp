#include "mcv_platform.h"
#include "OnAppearAction.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_transform.h"

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
	enemy->getSkeleton()->executeAction(animation, 0.1f, 0.1f);
	enemy->animationTimer.reset();
	EngineSound.emitEvent(sound, enemy->getTransform());
	return Leave;
}

void OnAppearAction::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
	sound = j.value("sound", sound);
}

void OnAppearAction::debugInMenu() {
	ImGui::Text("Animation: %s\n", animation.c_str());
}