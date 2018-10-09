#include "mcv_platform.h"
#include "AttackActionFlying.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_hitboxes.h"

REGISTER_BTACTION("AttackActionFlying", AttackActionFlying);

AttackActionFlying::AttackActionFlying() {
	type = "AttackActionFlying";
}

AttackActionFlying::AttackActionFlying(Enemy* enemy, std::string animation, std::string attack) :
	AttackActionFlying() {
	this->enemy = enemy;
	this->animation = animation;
	this->attack = attack;
}

int AttackActionFlying::execAction(float delta) {
	EnemyAttack enemyAttack = enemy->attacks[attack];
	if (enemy->animationTimer.elapsed() >= enemy->getSkeleton()->getAnimationDuration(animation)) {
		enemy->getHitboxes()->disable(enemyAttack.hitboxName);
		enemy->currentAttack = "";
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

void AttackActionFlying::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
	attack = j.value("attack", attack);
}

void AttackActionFlying::debugInMenu() {
	ImGui::Text("Animation: %s\n", animation.c_str());
	ImGui::Text("Attack: %s\n", attack.c_str());
}

void AttackActionFlying::onExit() {
	enemy->killParticles(enemy->attacks[attack].particles);
}