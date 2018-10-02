#include "mcv_platform.h"
#include "ShortDistanceCombatCondition.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTCONDITION("ShortDistanceCombatCondition", ShortDistanceCombatCondition);

ShortDistanceCombatCondition::ShortDistanceCombatCondition() {
	type = "ShortDistanceCombatCondition";
}

ShortDistanceCombatCondition::ShortDistanceCombatCondition(Enemy* enemy) :
	ShortDistanceCombatCondition() {
	this->enemy = enemy;
}

bool ShortDistanceCombatCondition::testCondition(float delta) {
	VEC3 playerPosition = enemy->getPlayerTransform()->getPosition();
	float distanceSqrd = VEC3::DistanceSquared(enemy->getTransform()->getPosition(), playerPosition);

	if (enemy->navMeshQuery) {
		return distanceSqrd <= enemy->shortCombatDistanceSqrd
			&& enemy->navMeshQuery->existsConnection(enemy->getTransform()->getPosition(), playerPosition);
	}
	else {
		return distanceSqrd <= enemy->shortCombatDistanceSqrd;
	}
}

void ShortDistanceCombatCondition::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}
