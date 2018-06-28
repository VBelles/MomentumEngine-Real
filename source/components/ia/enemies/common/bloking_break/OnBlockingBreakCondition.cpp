#include "mcv_platform.h"
#include "OnBlockingBreakCondition.h"
#include "components/ia/enemies/Enemy.h"

OnBlockingBreakCondition::OnBlockingBreakCondition(Enemy* enemy) : enemy(enemy) {
}

bool OnBlockingBreakCondition::testCondition(float delta) {
	return enemy->blockingBroken;
}
