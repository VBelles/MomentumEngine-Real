#include "mcv_platform.h"
#include "OnHitCondition.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTCONDITION("OnHitCondition", OnHitCondition);

OnHitCondition::OnHitCondition() {
	type = "OnHitCondition";
}

OnHitCondition::OnHitCondition(Enemy* enemy) :
	OnHitCondition() {
	this->enemy = enemy;
}

bool OnHitCondition::testCondition(float delta) {
	if (enemy->onHit) {
		enemy->onHit = false;
		return true;
	}
	else {
		return false;
	}
}

void OnHitCondition::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}