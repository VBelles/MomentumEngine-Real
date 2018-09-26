#include "mcv_platform.h"
#include "OnHorizontalLaunchCondition.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTCONDITION("OnHorizontalLaunchCondition", OnHorizontalLaunchCondition);

OnHorizontalLaunchCondition::OnHorizontalLaunchCondition() {
	type = "OnHorizontalLaunchCondition";
}

OnHorizontalLaunchCondition::OnHorizontalLaunchCondition(Enemy* enemy) :
	OnHorizontalLaunchCondition() {
	this->enemy = enemy;
}


bool OnHorizontalLaunchCondition::testCondition(float delta) {
	return enemy->receivedAttack.horizontalLauncher 
		&& !enemy->isBlocking 
		&& !enemy->hasSuperArmor();
}

void OnHorizontalLaunchCondition::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}