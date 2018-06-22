#include "mcv_platform.h"
#include "CombatCondition.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

CombatCondition::CombatCondition(Dreidel* dreidel): dreidel(dreidel) {
}

bool CombatCondition::testCondition(float delta) {
	return false;
}
