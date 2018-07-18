#include "mcv_platform.h"
#include "AttackAction.h"
#include "components/ia/enemies/Enemy.h"
#include "skeleton/comp_skeleton.h"
#include "components/comp_hitboxes.h"

REGISTER_BTAction("AttackAction", AttackAction);

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

void AttackAction::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
	attack = j.value("attack", attack);
}