#include "mcv_platform.h"
#include "OnBlockingBreakCondition.h"

OnBlockingBreakCondition::OnBlockingBreakCondition(Enemy* enemy) : enemy(enemy) {
}

bool OnBlockingBreakCondition::testCondition(float delta) {
	return enemy->blockingBroken;
}
