#include "mcv_platform.h"
#include "BlockCondition.h"
#include "components/ia/enemies/Enemy.h"

BlockCondition::BlockCondition(Enemy* enemy) : enemy(enemy) {
}

bool BlockCondition::testCondition(float delta) {
	return enemy->isBlocking;
}
