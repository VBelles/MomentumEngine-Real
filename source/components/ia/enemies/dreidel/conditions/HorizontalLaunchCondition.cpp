#include "mcv_platform.h"
#include "HorizontalLaunchCondition.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

HorizontalLaunchCondition::HorizontalLaunchCondition(Dreidel* dreidel): dreidel(dreidel) {
}

bool HorizontalLaunchCondition::testCondition(float delta) {
	return dreidel->receivedAttack.horizontalLauncher;
}
