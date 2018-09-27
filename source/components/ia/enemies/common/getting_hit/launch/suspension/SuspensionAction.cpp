#include "mcv_platform.h"
#include "SuspensionAction.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTACTION("SuspensionAction", SuspensionAction);

SuspensionAction::SuspensionAction() {
	type = "SuspensionAction";
}

SuspensionAction::SuspensionAction(Enemy* enemy) :
	SuspensionAction() {
	this->enemy = enemy;
}

int SuspensionAction::execAction(float delta) {
	if (enemy->timer.elapsed() < enemy->suspensionDuration) {
		return Stay;
	}
	return Leave;
}

void SuspensionAction::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}

void SuspensionAction::onExit() {
	enemy->getSound()->stop("launcher_loop");
	enemy->getSound()->play("launcher_end");
}
