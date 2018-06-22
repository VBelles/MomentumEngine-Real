#include "mcv_platform.h"
#include "CombatCondition.h"
#include "components/ia/enemies/dreidel/Dreidel.h"
#include "components/comp_transform.h"

CombatCondition::CombatCondition(Dreidel* dreidel) : dreidel(dreidel) {
}

bool CombatCondition::testCondition(float delta) {
	TCompTransform* transform = dreidel->getPlayerTransform();
	TCompTransform* playerTransform = dreidel->getPlayerTransform();
	float distanceSqrd = VEC3::DistanceSquared(transform->getPosition(), playerTransform->getPosition());
	return distanceSqrd < dreidel->maxCombatDistanceSqrd && transform->isInFov(playerTransform->getPosition(), dreidel->attackFov);
}
