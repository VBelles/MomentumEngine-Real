#include "mcv_platform.h"
#include "OnDisappearCondition.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTCONDITION("OnDisappearCondition", OnDisappearCondition);

OnDisappearCondition::OnDisappearCondition() {
	type = "OnDisappearCondition";
}

OnDisappearCondition::OnDisappearCondition(Enemy* enemy) :
	OnDisappearCondition() {
	this->enemy = enemy;
}

bool OnDisappearCondition::testCondition(float delta) {
	if (enemy->onDisappear) {
		enemy->onDisappear = false;
		return true;
	}
	else {
		return false;
	}
}

void OnDisappearCondition::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}
