#include "mcv_platform.h"
#include "StepBackCondition.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

StepBackCondition::StepBackCondition(Dreidel* dreidle): dreidel(dreidel) {
}

bool StepBackCondition::testCondition(float delta) {
	return false;
}
