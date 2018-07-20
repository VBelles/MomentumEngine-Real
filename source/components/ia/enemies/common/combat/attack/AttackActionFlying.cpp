#include "mcv_platform.h"
#include "AttackActionFlying.h"
#include "components/ia/enemies/Enemy.h"
#include "skeleton/comp_skeleton.h"
#include "components/comp_hitboxes.h"

REGISTER_BTACTION("AttackActionFlying", AttackActionFlying);

AttackActionFlying::AttackActionFlying() {
	type = "AttackActionFlying";
}

AttackActionFlying::AttackActionFlying(Enemy* enemy, std::string animation, std::string attack) : AttackActionFlying() {
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

void AttackActionFlying::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
	attack = j.value("attack", attack);
}