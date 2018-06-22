#include "mcv_platform.h"
#include "ChaseCondition.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

ChaseCondition::ChaseCondition(Dreidel* dreidle): dreidel(dreidel) {
}

bool ChaseCondition::testCondition(float delta) {
	return false;
}
