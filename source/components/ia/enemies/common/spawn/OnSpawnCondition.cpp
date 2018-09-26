#include "mcv_platform.h"
#include "OnSpawnCondition.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTCONDITION("OnSpawnCondition", OnSpawnCondition);

OnSpawnCondition::OnSpawnCondition() {
	type = "OnSpawnCondition";
}

OnSpawnCondition::OnSpawnCondition(Enemy* enemy) :
	OnSpawnCondition() {
	this->enemy = enemy;
}

bool OnSpawnCondition::testCondition(float delta) {
	if (enemy->onSpawn) {
		enemy->onSpawn = false;
		return true;
	}
	else {
		return false;
	}
}

void OnSpawnCondition::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}