#include "mcv_platform.h"
#include "RangedAttackAction.h"
#include "components/ia/enemies/Enemy.h"
#include "skeleton/comp_skeleton.h"
#include "components/comp_transform.h"
#include "entity/entity_parser.h"

RangedAttackAction::RangedAttackAction(Enemy* enemy, std::string animation, std::string attack, IBehaviorTreeCondition* cancelCondition) :
	enemy(enemy),
	animation(animation),
	attack(attack),
	cancelCondition(cancelCondition) {
}

int RangedAttackAction::execAction(float delta) {
	EnemyAttack enemyAttack = enemy->attacks[attack];

	if ((cancelCondition && cancelCondition->testCondition(delta))
		|| enemy->animationTimer.elapsed() >= enemy->getSkeleton()->getAnimationDuration(animation)) {
		attackLaunched = false;
		enemy->attackTimer.reset();
		return Leave;
	}
	else if (enemy->animationTimer.elapsed() >= frames2sec(enemyAttack.hitboxEnd)) {
		//Launch projectile
		if (!attackLaunched) {
			TEntityParseContext ctx;

			VEC3 offset = -enemy->getTransform()->getFront() * enemyAttack.attackSpawnOffset.z
				+ -enemy->getTransform()->getLeft() * enemyAttack.attackSpawnOffset.x;
			offset.y = enemyAttack.attackSpawnOffset.y;
			VEC3 attackInitialPos = enemy->getTransform()->getPosition() + offset;

			ctx.root_transform.setPosition(attackInitialPos);
			if (parseScene(enemyAttack.attackPrefab, ctx)) {
				assert(!ctx.entities_loaded.empty());

				AttackInfo attackInfo = enemyAttack.attackInfo;

				VEC3 attackDirection = enemy->getPlayerTransform()->getPosition() - attackInitialPos;
				attackDirection += enemyAttack.attackTargetOffset;
				attackDirection.Normalize();

				TMsgAssignRangedAttackOwner msg{enemy->getEntityHandle(), attackInfo, attackInitialPos, attackDirection };

				CEntity *attackEntity = ctx.entities_loaded[0];
				attackEntity->sendMsg(msg);
			}

			attackLaunched = true;
		}
		return Stay;
	}
	else if (enemy->animationTimer.elapsed() >= frames2sec(enemyAttack.hitboxStart)) {
		//TODO spawn projectile
		return Stay;
	}
	else {
		attackLaunched = false;
		return Stay;
	}
}
