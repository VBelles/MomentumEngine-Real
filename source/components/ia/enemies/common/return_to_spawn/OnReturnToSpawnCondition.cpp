#include "mcv_platform.h"
#include "OnReturnToSpawnCondition.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_transform.h"

OnReturnToSpawnCondition::OnReturnToSpawnCondition(Enemy* enemy) : enemy(enemy) {
}

bool OnReturnToSpawnCondition::testCondition(float delta) {
	return VEC3::DistanceSquared(enemy->spawnPosition, enemy->getTransform()->getPosition()) > enemy->strollRadius * enemy->strollRadius;
}
