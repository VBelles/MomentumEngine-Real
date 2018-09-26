#include "mcv_platform.h"
#include "BlockCondition.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTCONDITION("BlockCondition", BlockCondition);

BlockCondition::BlockCondition() {
	type = "BlockCondition";
}

BlockCondition::BlockCondition(Enemy* enemy) :
	BlockCondition() {
	this->enemy = enemy;
}

bool BlockCondition::testCondition(float delta) {
	return enemy->isBlocking;
}

void BlockCondition::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}