#include "mcv_platform.h"
#include "OnPropelCondition.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTCONDITION("OnPropelCondition", OnPropelCondition);

OnPropelCondition::OnPropelCondition(Enemy* enemy) : enemy(enemy) {
}

bool OnPropelCondition::testCondition(float delta) {
	return enemy->receivedAttack.propel 
		&& !enemy->isBlocking 
		&& !enemy->hasSuperArmor();
}

void OnPropelCondition::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}