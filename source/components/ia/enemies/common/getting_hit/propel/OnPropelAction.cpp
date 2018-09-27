#include "mcv_platform.h"
#include "OnPropelAction.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_hitboxes.h"

REGISTER_BTACTION("OnPropelAction", OnPropelAction);

OnPropelAction::OnPropelAction() {
	type = "OnPropelAction";
}

OnPropelAction::OnPropelAction(Enemy* enemy, std::string attack) :
	OnPropelAction() {
	this->enemy = enemy;
	this->attack = attack;
}

int OnPropelAction::execAction(float delta) {
	enemy->getCollider()->create(); //TODO enable/disable
	enemy->velocity = enemy->receivedAttack.propel->velocity;
	enemy->propelDuration = enemy->receivedAttack.propel->duration;
	enemy->timer.reset();
	enemy->currentAttack = attack;
	enemy->getHitboxes()->enable(enemy->attacks[attack].hitboxName);
	enemy->isBlocking = false;
	enemy->blockingBroken = false;
	return Leave;
}

void OnPropelAction::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	attack = j.value("attack", attack);
}

void OnPropelAction::debugInMenu() {
	ImGui::Text("Attack: %s\n", attack.c_str());
}
