#include "mcv_platform.h"
#include "TackleAttackAction.h"
#include "components/ia/enemies/Enemy.h"
#include "skeleton/comp_skeleton.h"
#include "components/comp_hitboxes.h"
#include "components/comp_transform.h"

TackleAttackAction::TackleAttackAction(Enemy* enemy, std::string animation, std::string attack, float speed, float time) :
	enemy(enemy),
	animation(animation),
	attack(attack),
	speed(speed),
	time(time) {
}

int TackleAttackAction::execAction(float delta) {
	enemy->updateGravity(delta);
	EnemyAttack enemyAttack = enemy->attacks[attack];
	enemy->deltaMovement += enemy->getTransform()->getFront() * speed * delta;
	if (enemy->timer.elapsed() >= time) {
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
