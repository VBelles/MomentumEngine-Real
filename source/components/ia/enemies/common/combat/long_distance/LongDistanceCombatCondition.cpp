#include "mcv_platform.h"
#include "LongDistanceCombatCondition.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_transform.h"

REGISTER_BTCONDITION("LongDistanceCombatCondition", LongDistanceCombatCondition);

LongDistanceCombatCondition::LongDistanceCombatCondition(Enemy* enemy) : enemy(enemy) {
}

bool LongDistanceCombatCondition::testCondition(float delta) {
	VEC3 playerPosition = enemy->getPlayerTransform()->getPosition();
	float distanceSqrd = VEC3::DistanceSquared(enemy->getTransform()->getPosition(), playerPosition);

	if (enemy->navMeshQuery) {
		return distanceSqrd <= enemy->longCombatDistanceSqrd
			&& enemy->navMeshQuery->existsConnection(enemy->getTransform()->getPosition(), playerPosition);
	}
	else {
		return distanceSqrd <= enemy->longCombatDistanceSqrd;
	}
}

void LongDistanceCombatCondition::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}
