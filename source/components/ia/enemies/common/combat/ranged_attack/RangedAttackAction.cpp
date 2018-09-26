#include "mcv_platform.h"
#include "RangedAttackAction.h"
#include "components/ia/enemies/Enemy.h"
#include "skeleton/comp_skeleton.h"
#include "components/comp_transform.h"
#include "entity/entity_parser.h"

REGISTER_BTACTION("RangedAttackAction", RangedAttackAction);

RangedAttackAction::RangedAttackAction() {
	type = "RangedAttackAction";
}

RangedAttackAction::RangedAttackAction(Enemy* enemy, std::string animation, std::string attack, std::string cancelCondition) :
	RangedAttackAction() {
	this->enemy = enemy;
	this->animation = animation;
	this->attack = attack;
	this->cancelCondition = cancelCondition;
}

int RangedAttackAction::execAction(float delta) {
	EnemyAttack enemyAttack = enemy->attacks[attack];

	if ((!cancelCondition.empty() && enemy->testCondition(cancelCondition, delta))
		|| enemy->animationTimer.elapsed() >= enemy->getSkeleton()->getAnimationDuration(animation)) {
		attackLaunched = false;
		enemy->attackTimer.reset();
		return Leave;
	}
	else if (enemy->animationTimer.elapsed() >= frames2sec(enemyAttack.hitboxEnd)) {
		return Stay;
	}
	else if (enemy->animationTimer.elapsed() >= frames2sec(enemyAttack.hitboxStart)) {
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

				TMsgAssignRangedAttackOwner msg{ enemy->getEntityHandle(), attackInfo, attackInitialPos, attackDirection, frames2sec(enemyAttack.hitboxEnd) };

				CEntity *attackEntity = ctx.entities_loaded[0];
				attackEntity->sendMsg(msg);
			}
			attackLaunched = true;
		}
		return Stay;
	}
	else {
		attackLaunched = false;
		return Stay;
	}
}

void RangedAttackAction::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
	
	attackLaunched = false;

	animation = j.value("animation", animation);
	attack = j.value("attack", attack);
	cancelCondition = j.value("cancel_condition", cancelCondition);
}

void RangedAttackAction::debugInMenu() {
	ImGui::Text("Animation: %s\n", animation.c_str());
	ImGui::Text("Attack: %s\n", attack.c_str());
	ImGui::Text("Cancel condition: %s\n", cancelCondition.c_str());
}