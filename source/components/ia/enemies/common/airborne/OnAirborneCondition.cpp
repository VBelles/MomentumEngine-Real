#include "mcv_platform.h"
#include "OnAirborneCondition.h"

OnAirborneCondition::OnAirborneCondition(Enemy* enemy) : enemy(enemy) {
}

bool OnAirborneCondition::testCondition(float delta) {
	return !enemy->airborne && !enemy->grounded;
}
