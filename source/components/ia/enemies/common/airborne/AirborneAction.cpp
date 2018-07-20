#include "mcv_platform.h"
#include "AirborneAction.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTACTION("AirborneAction", AirborneAction);

AirborneAction::AirborneAction() {
	type = "AirborneAction";
}

AirborneAction::AirborneAction(Enemy* enemy): AirborneAction() {
	this->enemy = enemy;
}

int AirborneAction::execAction(float delta) {
	enemy->updateGravity(delta);
	return Leave;
}

void AirborneAction::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}