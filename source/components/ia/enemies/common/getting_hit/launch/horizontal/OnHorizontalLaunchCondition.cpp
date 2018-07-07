#include "mcv_platform.h"
#include "OnHorizontalLaunchCondition.h"
#include "components/ia/enemies/Enemy.h"

OnHorizontalLaunchCondition::OnHorizontalLaunchCondition(Enemy* enemy) : enemy(enemy) {
}

bool OnHorizontalLaunchCondition::testCondition(float delta) {
	return enemy->receivedAttack.horizontalLauncher 
		&& !enemy->isBlocking 
		&& !enemy->hasSuperArmor();
}
