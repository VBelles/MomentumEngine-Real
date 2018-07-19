#include "mcv_platform.h"
#include "OnGrabAction.h"
#include "components/comp_collider.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTACTION("OnGrabAction", OnGrabAction);

OnGrabAction::OnGrabAction(Enemy* enemy): enemy(enemy) {
}

int OnGrabAction::execAction(float delta) {
	enemy->getCollider()->destroy(); //TODO enable/disable
	enemy->timer.reset();
	enemy->grabbedDuration = enemy->receivedAttack.grab->duration;
	enemy->isBlocking = false;
	enemy->blockingBroken = false;
	return Leave;
}

void OnGrabAction::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}