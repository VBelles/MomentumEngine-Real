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
	VEC3 movementIncrement = enemy->getTransform()->getFront() * speed * delta;
	VEC3 enemyPos = enemy->getTransform()->getPosition();
	if (enemy->timer.elapsed() >= time
		|| !(enemy->navMeshQuery && enemy->navMeshQuery->existsConnection(enemyPos, enemyPos + movementIncrement))) {
		enemy->getHitboxes()->disable(enemyAttack.hitboxName);
		enemy->currentAttack = "";
		return Leave;
	}
	else {
		enemy->getHitboxes()->disable(enemyAttack.hitboxName);
		if (enemy->animationTimer.elapsed() >= frames2sec(enemyAttack.hitboxEnd)) {
			enemy->deltaMovement += movementIncrement;

		}
		else if (enemy->animationTimer.elapsed() >= frames2sec(enemyAttack.hitboxStart)) {
			enemy->getHitboxes()->enable(enemyAttack.hitboxName);
		}
	}
	return Stay;
}
