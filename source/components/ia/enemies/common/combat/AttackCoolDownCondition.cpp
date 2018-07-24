#include "mcv_platform.h"
#include "AttackCoolDownCondition.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_transform.h"

REGISTER_BTCONDITION("AttackCoolDownCondition", AttackCoolDownCondition);

AttackCoolDownCondition::AttackCoolDownCondition() {
	type = "AttackCoolDownCondition";
}

AttackCoolDownCondition::AttackCoolDownCondition(Enemy* enemy) :
	AttackCoolDownCondition() {
	this->enemy = enemy;
}


bool AttackCoolDownCondition::testCondition(float delta) {
	return enemy->attackTimer.elapsed() < enemy->attackCoolDown;
}

void AttackCoolDownCondition::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}