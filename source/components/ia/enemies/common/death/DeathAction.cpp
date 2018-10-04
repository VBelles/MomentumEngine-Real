#include "mcv_platform.h"
#include "DeathAction.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTACTION("DeathAction", DeathAction);

DeathAction::DeathAction() {
	type = "DeathAction";
}

DeathAction::DeathAction(Enemy* enemy, std::string animation) :
	DeathAction() {
	this->enemy = enemy;
	this->animation = animation;
}

int DeathAction::execAction(float delta) {
	if (enemy->animationTimer.elapsed() < enemy->getSkeleton()->getAnimationDuration(animation)) {
		return Stay;
	}
	enemy->onDisappear = true;
	return Leave;
}

void DeathAction::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
}

void DeathAction::debugInMenu() {
	ImGui::Text("Animation: %s\n", animation.c_str());
}
