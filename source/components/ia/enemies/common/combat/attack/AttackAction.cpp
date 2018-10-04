#include "mcv_platform.h"
#include "AttackAction.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_hitboxes.h"

REGISTER_BTACTION("AttackAction", AttackAction);

AttackAction::AttackAction() {
	type = "AttackAction";
}

AttackAction::AttackAction(Enemy* enemy, std::string animation, std::string attack) :
	AttackAction() {
	this->enemy = enemy;
	this->animation = animation;
	this->attack = attack;
}

int AttackAction::execAction(float delta) {
	enemy->updateGravity(delta);
	EnemyAttack enemyAttack = enemy->attacks[attack];
	if (enemy->animationTimer.elapsed() >= enemy->getSkeleton()->getAnimationDuration(animation)
		|| enemy->animationTimer.elapsed() >= frames2sec(enemyAttack.attackEnd)) {
		enemy->getHitboxes()->disable(enemyAttack.hitboxName);
		enemy->currentAttack = "";
		enemy->attackTimer.reset();
		return Leave;
	}
	else if (enemy->animationTimer.elapsed() >= frames2sec(enemyAttack.hitboxEnd)) {
		enemy->getHitboxes()->disable(enemyAttack.hitboxName);
	}
	else if (enemy->animationTimer.elapsed() >= frames2sec(enemyAttack.hitboxStart)) {
		enemy->getHitboxes()->enable(enemyAttack.hitboxName);
	}

	return Stay;
}


void AttackAction::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
	animation = j.value("animation", animation);
	attack = j.value("attack", attack);
}

void AttackAction::debugInMenu() {
	ImGui::Text("Animation: %s\n", animation.c_str());
	ImGui::Text("Attack: %s\n", attack.c_str());
}

void AttackAction::onExit() {
	enemy->killParticles(enemy->attacks[attack].particles);
}
