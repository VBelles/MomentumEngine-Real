#include "mcv_platform.h"
#include "LongDistanceCombatCondition.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_transform.h"

LongDistanceCombatCondition::LongDistanceCombatCondition(Enemy* enemy) : enemy(enemy) {
}

bool LongDistanceCombatCondition::testCondition(float delta) {
	VEC3 playerPosition = enemy->getPlayerTransform()->getPosition();
	float distanceSqrd = VEC3::DistanceSquared(enemy->getTransform()->getPosition(), playerPosition);

	if (enemy->navmesh) {
		return distanceSqrd <= enemy->longCombatDistanceSqrd
			&& enemy->navmesh->existsConnection(enemy->getTransform()->getPosition(), playerPosition);
	}
	else {
		return distanceSqrd <= enemy->longCombatDistanceSqrd;
	}
}
