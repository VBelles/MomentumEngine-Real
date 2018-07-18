#include "mcv_platform.h"
#include "BlockCondition.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTCondition("BlockCondition", BlockCondition);

BlockCondition::BlockCondition(Enemy* enemy) : enemy(enemy) {
}

bool BlockCondition::testCondition(float delta) {
	return enemy->isBlocking;
}

void BlockCondition::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}