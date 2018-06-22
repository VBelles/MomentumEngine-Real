#include "mcv_platform.h"
#include "ChaseCondition.h"
#include "components/ia/enemies/dreidel/Dreidel.h"
#include "components/comp_transform.h"

ChaseCondition::ChaseCondition(Dreidel* dreidel): dreidel(dreidel) {
}

bool ChaseCondition::testCondition(float delta) {
	TCompTransform* transform = dreidel->getTransform();
	TCompTransform* playerTransform = dreidel->getPlayerTransform();
	float distanceSqrd = VEC3::DistanceSquared(transform->getPosition(), playerTransform->getPosition());
	bool isPlayerInFov = transform->isInFov(playerTransform->getPosition(), dreidel->chaseFov);
	return distanceSqrd > dreidel->minCombatDistanceSqrd
		&& (distanceSqrd < dreidel->smallChaseRadiusSqrd || (distanceSqrd < dreidel->fovChaseDistanceSqrd && isPlayerInFov));
}
