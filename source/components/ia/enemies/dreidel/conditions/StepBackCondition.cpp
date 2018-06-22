#include "mcv_platform.h"
#include "StepBackCondition.h"
#include "components/ia/enemies/dreidel/Dreidel.h"
#include "components/comp_transform.h"

StepBackCondition::StepBackCondition(Dreidel* dreidel): dreidel(dreidel) {
}

bool StepBackCondition::testCondition(float delta) {
	VEC3 myPos = dreidel->getTransform()->getPosition();
	VEC3 playerPos = dreidel->getPlayerTransform()->getPosition();
	myPos.y = 0;
	playerPos.y = 0;
	float distanceSqrd = VEC3::DistanceSquared(myPos, playerPos);
	return distanceSqrd <  dreidel->minCombatDistanceSqrd;
}
