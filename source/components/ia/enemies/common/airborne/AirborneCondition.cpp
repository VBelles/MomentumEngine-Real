#include "mcv_platform.h"
#include "AirborneCondition.h"
#include "components/ia/enemies/Enemy.h"

AirborneCondition::AirborneCondition(Enemy* enemy) : enemy(enemy) {
}

bool AirborneCondition::testCondition(float delta) {
	return enemy->airborne;
}
