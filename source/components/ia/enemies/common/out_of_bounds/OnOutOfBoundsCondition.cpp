#include "mcv_platform.h"
#include "OnOutOfBoundsCondition.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTCONDITION("OnOutOfBoundsCondition", OnOutOfBoundsCondition);

OnOutOfBoundsCondition::OnOutOfBoundsCondition() {
	type = "OnOutOfBoundsCondition";
}

OnOutOfBoundsCondition::OnOutOfBoundsCondition(Enemy* enemy) :
	OnOutOfBoundsCondition() {
	this->enemy = enemy;
}

bool OnOutOfBoundsCondition::testCondition(float delta) {
	if (enemy->onOutOfBounds) {
		enemy->onOutOfBounds = false;
		return true;
	}
	else {
		return false;
	}
}

void OnOutOfBoundsCondition::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}