#include "mcv_platform.h"
#include "VerticalLaunchedAction.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTACTION("VerticalLaunchedAction", VerticalLaunchedAction);

VerticalLaunchedAction::VerticalLaunchedAction(Enemy* enemy) : enemy(enemy) {
}

int VerticalLaunchedAction::execAction(float delta) {
	enemy->updateGravity(delta);
	if (enemy->velocity.y <= 0) {
		enemy->timer.reset();
		return Leave;
	}
	else {
		return Stay;
	}
}

void VerticalLaunchedAction::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}