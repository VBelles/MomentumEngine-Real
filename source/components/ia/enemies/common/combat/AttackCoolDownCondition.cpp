#include "mcv_platform.h"
#include "AttackCoolDownCondition.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_transform.h"

AttackCoolDownCondition::AttackCoolDownCondition(Enemy* enemy) : enemy(enemy) {
}

bool AttackCoolDownCondition::testCondition(float delta) {
	return enemy->attackTimer.elapsed() < enemy->attackCoolDown;
}
