#include "mcv_platform.h"
#include "OnGrabCondition.h"
#include "components/ia/enemies/Enemy.h"

OnGrabCondition::OnGrabCondition(Enemy* enemy) : enemy(enemy) {
}

bool OnGrabCondition::testCondition(float delta) {
	return enemy->receivedAttack.grab 
		&& !enemy->isBlocking 
		&& !enemy->hasSuperArmor();
}
