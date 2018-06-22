#include "mcv_platform.h"
#include "PropelCondition.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

PropelCondition::PropelCondition(Dreidel* dreidle): dreidel(dreidel) {
}

bool PropelCondition::testCondition(float delta) {
	return false;
}
