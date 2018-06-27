#include "mcv_platform.h"
#include "OnPropelCondition.h"

OnPropelCondition::OnPropelCondition(Enemy* enemy) : enemy(enemy) {
}

bool OnPropelCondition::testCondition(float delta) {
	return enemy->receivedAttack.propel 
		&& !enemy->isBlocking 
		&& !enemy->hasSuperArmor();
}
