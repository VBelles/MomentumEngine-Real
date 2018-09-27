#include "mcv_platform.h"
#include "OnReturnToSpawnCondition.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTCONDITION("OnReturnToSpawnCondition", OnReturnToSpawnCondition);

OnReturnToSpawnCondition::OnReturnToSpawnCondition() {
	type = "OnReturnToSpawnCondition";
}

OnReturnToSpawnCondition::OnReturnToSpawnCondition(Enemy* enemy) :
	OnReturnToSpawnCondition() {
	this->enemy = enemy;
}

bool OnReturnToSpawnCondition::testCondition(float delta) {
	return VEC3::DistanceSquared(enemy->spawnPosition, enemy->getTransform()->getPosition()) > enemy->strollRadius * enemy->strollRadius;
}

void OnReturnToSpawnCondition::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}
