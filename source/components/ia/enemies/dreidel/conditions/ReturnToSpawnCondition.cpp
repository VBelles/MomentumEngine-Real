#include "mcv_platform.h"
#include "ReturnToSpawnCondition.h"
#include "components/ia/enemies/dreidel/Dreidel.h"
#include "components/comp_transform.h"

ReturnToSpawnCondition::ReturnToSpawnCondition(Dreidel* dreidel): dreidel(dreidel) {
}

bool ReturnToSpawnCondition::testCondition(float delta) {
	return VEC3::DistanceSquared(dreidel->getTransform()->getPosition(), dreidel->spawnPosition) > dreidel->recallDistanceSqrd;
}
