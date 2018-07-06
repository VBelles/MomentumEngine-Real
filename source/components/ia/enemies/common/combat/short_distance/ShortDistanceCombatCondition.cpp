#include "mcv_platform.h"
#include "ShortDistanceCombatCondition.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_transform.h"

ShortDistanceCombatCondition::ShortDistanceCombatCondition(Enemy* enemy) : enemy(enemy) {
}

bool ShortDistanceCombatCondition::testCondition(float delta) {
	VEC3 playerPosition = enemy->getPlayerTransform()->getPosition();
	float distanceSqrd = VEC3::DistanceSquared(enemy->getTransform()->getPosition(), playerPosition);

	if (enemy->navmesh) {
		return distanceSqrd <= enemy->shortCombatDistanceSqrd
			&& enemy->navmesh->existsConnection(enemy->getTransform()->getPosition(), playerPosition);
	}
	else {
		return distanceSqrd <= enemy->shortCombatDistanceSqrd;
	}
}
