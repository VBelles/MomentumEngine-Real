#include "mcv_platform.h"
#include "OnDeathAction.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_transform.h"

REGISTER_BTACTION("OnDeathAction", OnDeathAction);

OnDeathAction::OnDeathAction() {
	type = "OnDeathAction";
}

OnDeathAction::OnDeathAction(Enemy* enemy, std::string animation) :
	OnDeathAction() {
	this->enemy = enemy;
	this->animation = animation;
}

int OnDeathAction::execAction(float delta) {
	enemy->isBlocking = false;
	enemy->blockingBroken = false;
	enemy->getSkeleton()->executeAction(animation, 0.1f, 0.1f);
	enemy->animationTimer.reset();
	EngineSound.emitEvent(sound, enemy->getTransform());
	return Leave;
}

void OnDeathAction::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
	sound = j.value("sound", sound);
}

void OnDeathAction::debugInMenu() {
	ImGui::Text("Animation: %s\n", animation.c_str());
}