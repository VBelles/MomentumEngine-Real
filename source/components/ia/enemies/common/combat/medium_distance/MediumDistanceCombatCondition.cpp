#include "mcv_platform.h"
#include "MediumDistanceCombatCondition.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_transform.h"

REGISTER_BTCONDITION("MediumDistanceCombatCondition", MediumDistanceCombatCondition);

MediumDistanceCombatCondition::MediumDistanceCombatCondition() {
	type = "MediumDistanceCombatCondition";
}

MediumDistanceCombatCondition::MediumDistanceCombatCondition(Enemy* enemy) :
	MediumDistanceCombatCondition() {
	this->enemy = enemy;
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

void MediumDistanceCombatCondition::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}