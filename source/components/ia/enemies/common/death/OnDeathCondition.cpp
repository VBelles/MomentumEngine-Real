#include "mcv_platform.h"
#include "OnDeathCondition.h"
#include "components/ia/enemies/Enemy.h"

OnDeathCondition::OnDeathCondition(Enemy* enemy) : enemy(enemy) {
}

bool OnDeathCondition::testCondition(float delta) {
	return enemy->hp <= 0.f;
}
