#include "mcv_platform.h"
#include "OnHorizontalLaunchCondition.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTCONDITION("OnHorizontalLaunchCondition", OnHorizontalLaunchCondition);

OnHorizontalLaunchCondition::OnHorizontalLaunchCondition(Enemy* enemy) : enemy(enemy) {
}

bool OnHorizontalLaunchCondition::testCondition(float delta) {
	return enemy->receivedAttack.horizontalLauncher 
		&& !enemy->isBlocking 
		&& !enemy->hasSuperArmor();
}

void OnHorizontalLaunchCondition::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}