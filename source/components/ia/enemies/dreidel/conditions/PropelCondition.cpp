#include "mcv_platform.h"
#include "PropelCondition.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

PropelCondition::PropelCondition(Dreidel* dreidel): dreidel(dreidel) {
}

bool PropelCondition::testCondition(float delta) {
	return dreidel->receivedAttack.propel;
}
