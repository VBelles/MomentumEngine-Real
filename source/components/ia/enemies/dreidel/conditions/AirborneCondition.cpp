#include "mcv_platform.h"
#include "AirborneCondition.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

AirborneCondition::AirborneCondition(Dreidel* dreidel): dreidel(dreidel) {
}

bool AirborneCondition::testCondition(float delta) {
	return false;
}
