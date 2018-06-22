#include "mcv_platform.h"
#include "DeathCondition.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

DeathCondition::DeathCondition(Dreidel* dreidle): dreidel(dreidel) {
}

bool DeathCondition::testCondition(float delta) {
	return false;
}
