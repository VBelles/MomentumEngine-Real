#include "mcv_platform.h"
#include "OnReturnToSpawnCondition.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_transform.h"

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

void OnReturnToSpawnCondition::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}