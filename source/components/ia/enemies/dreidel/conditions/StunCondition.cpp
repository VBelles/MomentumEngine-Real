#include "mcv_platform.h"
#include "StunCondition.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

StunCondition::StunCondition(Dreidel* dreidel): dreidel(dreidel) {
}

bool StunCondition::testCondition(float delta) {
	return false;
}
