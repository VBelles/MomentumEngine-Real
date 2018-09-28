#include "mcv_platform.h"
#include "OnVerticalLaunchCondition.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTCONDITION("OnVerticalLaunchCondition", OnVerticalLaunchCondition);

OnVerticalLaunchCondition::OnVerticalLaunchCondition() {
	type = "OnVerticalLaunchCondition";
}

OnVerticalLaunchCondition::OnVerticalLaunchCondition(Enemy* enemy) :
	OnVerticalLaunchCondition() {
	this->enemy = enemy;
}

bool OnVerticalLaunchCondition::testCondition(float delta) {
	return enemy->receivedAttack.verticalLauncher 
		&& !enemy->isBlocking 
		&& !enemy->hasSuperArmor();
}

void OnVerticalLaunchCondition::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}
