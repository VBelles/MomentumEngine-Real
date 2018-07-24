#include "mcv_platform.h"
#include "AirborneCondition.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTCONDITION("AirborneCondition", AirborneCondition);

AirborneCondition::AirborneCondition() {
	type = "AirborneCondition";
}

AirborneCondition::AirborneCondition(Enemy* enemy) :
	AirborneCondition() {
	this->enemy = enemy;
}

bool AirborneCondition::testCondition(float delta) {
	return enemy->airborne;
}

void AirborneCondition::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}