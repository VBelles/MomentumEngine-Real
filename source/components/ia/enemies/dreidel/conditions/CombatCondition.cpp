#include "mcv_platform.h"
#include "CombatCondition.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

CombatCondition::CombatCondition(Dreidel* dreidle): dreidel(dreidel) {
}

bool CombatCondition::testCondition(float delta) {
	return false;
}
