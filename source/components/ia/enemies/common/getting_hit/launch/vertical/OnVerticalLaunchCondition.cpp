#include "mcv_platform.h"
#include "OnVerticalLaunchCondition.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTCONDITION("OnVerticalLaunchCondition", OnVerticalLaunchCondition);

OnVerticalLaunchCondition::OnVerticalLaunchCondition(Enemy* enemy) : enemy(enemy) {
}

bool OnVerticalLaunchCondition::testCondition(float delta) {
	return enemy->receivedAttack.verticalLauncher 
		&& !enemy->isBlocking 
		&& !enemy->hasSuperArmor();
}

void OnVerticalLaunchCondition::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}