#include "mcv_platform.h"
#include "OnDeathCondition.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_give_power.h"

REGISTER_BTCONDITION("OnDeathCondition", OnDeathCondition);

OnDeathCondition::OnDeathCondition(Enemy* enemy) : enemy(enemy) {
}

bool OnDeathCondition::testCondition(float delta) {
	return enemy->hp <= 0.f || enemy->getPower()->getPowerToGive() <= 0;
}

void OnDeathCondition::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}