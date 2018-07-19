#include "mcv_platform.h"
#include "OnBlockingBreakCondition.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTCONDITION("OnBlockingBreakCondition", OnBlockingBreakCondition);

OnBlockingBreakCondition::OnBlockingBreakCondition(Enemy* enemy) : enemy(enemy) {
}

bool OnBlockingBreakCondition::testCondition(float delta) {
	return enemy->blockingBroken;
}

void OnBlockingBreakCondition::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}