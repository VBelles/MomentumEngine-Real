#include "mcv_platform.h"
#include "AttackAction.h"
#include "components/ia/enemies/Enemy.h"
#include "skeleton/comp_skeleton.h"
#include "components/comp_hitboxes.h"

AttackAction::AttackAction(Enemy* enemy, std::string animation, std::string attack) :
	enemy(enemy),
	animation(animation),
	attack(attack) {
}

int AttackAction::execAction(float delta) {
	enemy->updateGravity(delta);
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
