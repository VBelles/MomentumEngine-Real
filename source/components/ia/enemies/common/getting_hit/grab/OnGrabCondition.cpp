#include "mcv_platform.h"
#include "OnGrabCondition.h"

OnGrabCondition::OnGrabCondition(Enemy* enemy) : enemy(enemy) {
}

bool OnGrabCondition::testCondition(float delta) {
	return enemy->receivedAttack.grab 
		&& !enemy->isBlocking 
		&& !enemy->hasSuperArmor();
}
