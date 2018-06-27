#include "mcv_platform.h"
#include "OnVerticalLaunchCondition.h"

OnVerticalLaunchCondition::OnVerticalLaunchCondition(Enemy* enemy) : enemy(enemy) {
}

bool OnVerticalLaunchCondition::testCondition(float delta) {
	return enemy->receivedAttack.verticalLauncher 
		&& !enemy->isBlocking 
		&& !enemy->hasSuperArmor();
}
