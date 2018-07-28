#include "mcv_platform.h"
#include "AddAttackerCondition.h"
#include "components/ia/enemies/Enemy.h"
#include "components/player/comp_player_model.h"

REGISTER_BTCONDITION("AddAttackerCondition", AddAttackerCondition);

AddAttackerCondition::AddAttackerCondition() {
	type = "AddAttackerCondition";
}

AddAttackerCondition::AddAttackerCondition(Enemy* enemy) :
	AddAttackerCondition() {
	this->enemy = enemy;
}

bool AddAttackerCondition::testCondition(float delta) {
	if (enemy->attackTarget.empty()) {
		CEntity* entity = enemy->getEntityHandle();
		if (enemy->getPlayerModel()->addAttacker(entity->getName(), enemy->attackSlots)) {
			enemy->attackTarget = PLAYER_NAME;
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return true;
	}
}

void AddAttackerCondition::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}