#include "mcv_platform.h"
#include "OnTeleportCondition.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_transform.h"

OnTeleportCondition::OnTeleportCondition(Enemy* enemy) : enemy(enemy) {
}

bool OnTeleportCondition::testCondition(float delta) {
	if (enemy->navmesh) {
		return VEC3::DistanceSquared(enemy->spawnPosition, enemy->getTransform()->getPosition()) > enemy->maxDistanceFromSpawnSqrd
			|| !enemy->navmesh->existsConnection(enemy->getTransform()->getPosition(), enemy->spawnPosition);
	}
	else {
		return VEC3::DistanceSquared(enemy->spawnPosition, enemy->getTransform()->getPosition()) > enemy->maxDistanceFromSpawnSqrd;
	}
}
