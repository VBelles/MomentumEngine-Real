#include "mcv_platform.h"
#include "OnStunCondition.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTCONDITION("OnStunCondition", OnStunCondition);

OnStunCondition::OnStunCondition(Enemy* enemy) : enemy(enemy) {
}

bool OnStunCondition::testCondition(float delta) {
	return enemy->stunTimer.elapsed() < enemy->stunDuration;
}

void OnStunCondition::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}