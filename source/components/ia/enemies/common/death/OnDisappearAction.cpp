#include "mcv_platform.h"
#include "OnDisappearAction.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_transform.h"

REGISTER_BTACTION("OnDisappearAction", OnDisappearAction);

OnDisappearAction::OnDisappearAction() {
	type = "OnDisappearAction";
}

OnDisappearAction::OnDisappearAction(Enemy* enemy, std::string animation) :
	OnDisappearAction() {
	this->enemy = enemy;
	this->animation = animation;
}

int OnDisappearAction::execAction(float delta) {
	enemy->getSkeleton()->executeAction(animation, 0.1f, 0.1f);
	enemy->animationTimer.reset();
	EngineSound.emitEvent(sound, enemy->getTransform());
	return Leave;
}

void OnDisappearAction::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
	sound = j.value("sound", sound);
}

void OnDisappearAction::debugInMenu() {
	ImGui::Text("Animation: %s\n", animation.c_str());
}