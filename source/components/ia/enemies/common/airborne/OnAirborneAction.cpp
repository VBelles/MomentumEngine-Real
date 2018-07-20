#include "mcv_platform.h"
#include "OnAirborneAction.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTACTION("OnAirborneAction", OnAirborneAction);

OnAirborneAction::OnAirborneAction() {
	type = "OnAirborneAction";
}

OnAirborneAction::OnAirborneAction(Enemy* enemy, std::string animation) : OnAirborneAction() {
	this->enemy = enemy;
	this->animation = animation;
}

int OnAirborneAction::execAction(float delta) {
	enemy->updateGravity(delta);
	enemy->getSkeleton()->blendCycle(animation, 0.1f, 0.1f);
	enemy->airborne = true;
	return Leave;
}

void OnAirborneAction::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
}