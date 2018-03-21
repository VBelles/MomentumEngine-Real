#include "mcv_platform.h"
#include "BehaviorTreeMeleeEnemy.h"

DECL_OBJ_MANAGER("behaviorTree_melee_enemy", CBehaviorTreeMeleeEnemy);

CBehaviorTreeMeleeEnemy::CBehaviorTreeMeleeEnemy()
	: IBehaviorTree::IBehaviorTree() {
}

void CBehaviorTreeMeleeEnemy::load(const json& j, TEntityParseContext& ctx) {
}

void CBehaviorTreeMeleeEnemy::debugInMenu() {
}

void CBehaviorTreeMeleeEnemy::registerMsgs() {
}

void CBehaviorTreeMeleeEnemy::update(float delta) {
}