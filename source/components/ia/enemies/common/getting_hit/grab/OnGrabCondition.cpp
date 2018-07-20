#include "mcv_platform.h"
#include "OnGrabCondition.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTCONDITION("OnGrabCondition", OnGrabCondition);

OnGrabCondition::OnGrabCondition() {
	type = "OnGrabCondition";
}

OnGrabCondition::OnGrabCondition(Enemy* enemy) :
	OnGrabCondition() {
	this->enemy = enemy;
}

bool OnGrabCondition::testCondition(float delta) {
	return enemy->receivedAttack.grab 
		&& !enemy->isBlocking 
		&& !enemy->hasSuperArmor();
}

void OnGrabCondition::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}