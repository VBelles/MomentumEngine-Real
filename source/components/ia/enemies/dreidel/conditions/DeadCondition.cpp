#include "mcv_platform.h"
#include "DeadCondition.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

DeadCondition::DeadCondition(Dreidel* dreidel): dreidel(dreidel) {
}

bool DeadCondition::testCondition(float delta) {
	return false;
}
