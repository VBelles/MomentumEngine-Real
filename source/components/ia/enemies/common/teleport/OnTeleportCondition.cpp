#include "mcv_platform.h"
#include "OnTeleportCondition.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTCONDITION("OnTeleportCondition", OnTeleportCondition);

OnTeleportCondition::OnTeleportCondition() {
	type = "OnTeleportCondition";
}

OnTeleportCondition::OnTeleportCondition(Enemy* enemy) :
	OnTeleportCondition() {
	this->enemy = enemy;
}

bool OnTeleportCondition::testCondition(float delta) {
	if (enemy->navMeshQuery) {
		return VEC3::DistanceSquared(enemy->spawnPosition, enemy->getTransform()->getPosition()) > enemy->maxDistanceFromSpawnSqrd
			|| !enemy->navMeshQuery->existsConnection(enemy->getTransform()->getPosition(), enemy->spawnPosition);
	}
	else {
		return VEC3::DistanceSquared(enemy->spawnPosition, enemy->getTransform()->getPosition()) > enemy->maxDistanceFromSpawnSqrd;
	}
}

void OnTeleportCondition::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}
