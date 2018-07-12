#include "mcv_platform.h"
#include "RangedAttackAction.h"
#include "components/ia/enemies/kippah/Kippah.h"
#include "skeleton/comp_skeleton.h"
#include "components/comp_hitboxes.h"
#include "components/comp_transform.h"
#include "entity/entity_parser.h"

RangedAttackAction::RangedAttackAction(Kippah* enemy, std::string animation, std::string attack, IBehaviorTreeCondition* cancelCondition) :
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
		return Leave;
	}
	else if (enemy->animationTimer.elapsed() >= frames2sec(enemyAttack.hitboxEnd)) {
		//Launch projectile
		if (!attackLaunched) {
			attackLaunched = true;
			TEntityParseContext ctx;

			VEC3 offset = -enemy->getTransform()->getFront() * enemy->attackSpawnOffset.z
				+ -enemy->getTransform()->getLeft() * enemy->attackSpawnOffset.x;
			offset.y = enemy->attackSpawnOffset.y;
			VEC3 attackInitialPos = enemy->getTransform()->getPosition() + offset;

			ctx.root_transform.setPosition(attackInitialPos);
			if (parseScene(enemy->attackPrefab, ctx)) {
				assert(!ctx.entities_loaded.empty());

				AttackInfo attackInfo = enemyAttack.attackInfo;

				VEC3 attackDirection = enemy->getPlayerTransform()->getPosition() - attackInitialPos;
				attackDirection += enemy->attackTargetOffset;
				attackDirection.Normalize();

				TMsgAssignRangedAttackOwner msg{ CHandle(enemy).getOwner(), attackInfo, attackInitialPos, attackDirection };

				CEntity *attackEntity = ctx.entities_loaded[0];
				attackEntity->sendMsg(msg);
			}
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
