#include "mcv_platform.h"
#include "VerticalLaunchCondition.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

VerticalLaunchCondition::VerticalLaunchCondition(Dreidel* dreidel): dreidel(dreidel) {
}

bool VerticalLaunchCondition::testCondition(float delta) {
	return dreidel->receivedAttack.verticalLauncher;
}
