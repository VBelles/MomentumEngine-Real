#include "mcv_platform.h"
#include "VerticalLaunchedAction.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_hitboxes.h"

REGISTER_BTACTION("VerticalLaunchedAction", VerticalLaunchedAction);

VerticalLaunchedAction::VerticalLaunchedAction() {
	type = "VerticalLaunchedAction";
}

VerticalLaunchedAction::VerticalLaunchedAction(Enemy* enemy, std::string attack) :
	VerticalLaunchedAction() {
	this->enemy = enemy;
	this->attack = attack;
}

int VerticalLaunchedAction::execAction(float delta) {
	enemy->updateGravity(delta);
	if (enemy->velocity.y <= 0) {
		enemy->timer.reset();
		enemy->getHitboxes()->disable(enemy->attacks[attack].hitboxName);
		enemy->currentAttack = "";
		return Leave;
	}
	else {
		return Stay;
	}
}

void VerticalLaunchedAction::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	attack = j.value("attack", attack);
}

void VerticalLaunchedAction::debugInMenu() {
	ImGui::Text("Attack: %s\n", attack.c_str());
}