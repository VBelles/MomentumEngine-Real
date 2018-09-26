#include "mcv_platform.h"
#include "OnSuspensionCondition.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTCONDITION("OnSuspensionCondition", OnSuspensionCondition);

OnSuspensionCondition::OnSuspensionCondition() {
	type = "OnSuspensionCondition";
}

OnSuspensionCondition::OnSuspensionCondition(Enemy* enemy) :
	OnSuspensionCondition() {
	this->enemy = enemy;
}

bool OnSuspensionCondition::testCondition(float delta) {
	return enemy->timer.elapsed() < enemy->suspensionDuration;
}

void OnSuspensionCondition::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}