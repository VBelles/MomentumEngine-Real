#include "mcv_platform.h"
#include "OnDeathCondition.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_give_power.h"

REGISTER_BTCONDITION("OnDeathCondition", OnDeathCondition);

OnDeathCondition::OnDeathCondition() {
	type = "OnDeathCondition";
}

OnDeathCondition::OnDeathCondition(Enemy* enemy) :
	OnDeathCondition() {
	this->enemy = enemy;
}

bool OnDeathCondition::testCondition(float delta) {
	return enemy->hp <= 0.f || enemy->getPower()->getPowerToGive() <= 0;
}

void OnDeathCondition::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}