#include "mcv_platform.h"
#include "OnPropelCondition.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTCONDITION("OnPropelCondition", OnPropelCondition);

OnPropelCondition::OnPropelCondition() {
	type = "OnPropelCondition";
}

OnPropelCondition::OnPropelCondition(Enemy* enemy) :
	OnPropelCondition() {
	this->enemy = enemy;
}

bool OnPropelCondition::testCondition(float delta) {
	return enemy->receivedAttack.propel 
		&& !enemy->isBlocking 
		&& !enemy->hasSuperArmor();
}

void OnPropelCondition::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}
