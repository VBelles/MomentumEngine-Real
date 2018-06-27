#include "mcv_platform.h"
#include "AirborneCondition.h"
#include "components/ia/enemies/dreidel/Dreidel.h"
#include "..\..\common\conditions\airborne\AirborneCondition.h"



bool AirborneCondition::testCondition(float delta) {
	return !dreidel->grounded;
}

AirborneCondition::AirborneCondition(Enemy* enemy): enemy(enemy) {
}
