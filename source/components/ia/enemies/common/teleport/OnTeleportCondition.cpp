#include "mcv_platform.h"
#include "OnTeleportCondition.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_transform.h"

OnTeleportCondition::OnTeleportCondition(Enemy* enemy) : enemy(enemy) {
}

bool OnTeleportCondition::testCondition(float delta) {
	return VEC3::DistanceSquared(enemy->spawnPosition, enemy->getTransform()->getPosition()) > enemy->maxDistanceFromSpawnSqrd;
}
