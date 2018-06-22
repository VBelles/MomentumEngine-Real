#include "mcv_platform.h"
#include "HorizontalLaunchCondition.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

HorizontalLaunchCondition::HorizontalLaunchCondition(Dreidel* dreidle): dreidel(dreidel) {
}

bool HorizontalLaunchCondition::testCondition(float delta) {
	return false;
}
