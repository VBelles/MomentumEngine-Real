#include "mcv_platform.h"
#include "PropelAction.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_hitboxes.h"

REGISTER_BTACTION("PropelAction", PropelAction);

PropelAction::PropelAction() {
	type = "PropelAction";
}

PropelAction::PropelAction(Enemy* enemy, std::string attack) :
	PropelAction() {
	this->enemy = enemy;
	this->attack = attack;
}


int PropelAction::execAction(float delta) {
	if (enemy->timer.elapsed() < enemy->propelDuration) {
		enemy->deltaMovement += enemy->velocity * delta;
		enemy->updateGravity(delta);
		return Stay;
	}
	else {
		enemy->getHitboxes()->disable(enemy->attacks[attack].hitboxName);
		enemy->currentAttack = "";
		return Leave;
	}
}

void PropelAction::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	attack = j.value("attack", attack);
}

void PropelAction::debugInMenu() {
	ImGui::Text("Attack: %s\n", attack.c_str());
}