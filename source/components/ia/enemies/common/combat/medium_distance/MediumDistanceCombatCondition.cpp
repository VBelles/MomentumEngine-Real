#include "mcv_platform.h"
#include "MediumDistanceCombatCondition.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_transform.h"

MediumDistanceCombatCondition::MediumDistanceCombatCondition(Enemy* enemy) : enemy(enemy) {
}

bool MediumDistanceCombatCondition::testCondition(float delta) {
	VEC3 playerPosition = enemy->getPlayerTransform()->getPosition();
	float distanceSqrd = VEC3::DistanceSquared(enemy->getTransform()->getPosition(), playerPosition);

	if (enemy->navMeshQuery) {
		return distanceSqrd <= enemy->mediumCombatDistanceSqrd
			&& enemy->navMeshQuery->existsConnection(enemy->getTransform()->getPosition(), playerPosition);
	}
	else {
		return distanceSqrd <= enemy->mediumCombatDistanceSqrd;
	}
}
