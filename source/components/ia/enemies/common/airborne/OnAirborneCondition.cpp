#include "mcv_platform.h"
#include "OnAirborneCondition.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTCondition("OnAirborneCondition", OnAirborneCondition);

OnAirborneCondition::OnAirborneCondition(Enemy* enemy) : enemy(enemy) {
}

bool OnAirborneCondition::testCondition(float delta) {
	return !enemy->airborne && !enemy->grounded;
}

void OnAirborneCondition::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}