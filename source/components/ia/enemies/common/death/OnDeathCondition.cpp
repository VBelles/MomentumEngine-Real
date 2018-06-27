#include "mcv_platform.h"
#include "OnDeathCondition.h"

OnDeathCondition::OnDeathCondition(Enemy* enemy) : enemy(enemy) {
}

bool OnDeathCondition::testCondition(float delta) {
	return enemy->hp <= 0.f;
}
