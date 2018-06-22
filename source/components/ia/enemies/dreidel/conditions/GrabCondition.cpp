#include "mcv_platform.h"
#include "GrabCondition.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

GrabCondition::GrabCondition(Dreidel* dreidle): dreidel(dreidel) {
}

bool GrabCondition::testCondition(float delta) {
	return false;
}
