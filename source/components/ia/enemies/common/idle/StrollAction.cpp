#include "mcv_platform.h"
#include "StrollAction.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_transform.h"
#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

StrollAction::StrollAction(Enemy* enemy, IBehaviorTreeCondition* combatCondition) :
	enemy(enemy),
	combatCondition(combatCondition) {
}

int StrollAction::execAction(float delta) {
	enemy->updateGravity(delta);
	float movementIncrement = enemy->movementSpeed * delta;
	if (combatCondition->testCondition(delta) || enemy->currentPathPoint >= enemy->smoothPath.size()) {
		return Leave;
	}
	else if (VEC3::DistanceSquared(enemy->smoothPath[enemy->currentPathPoint], enemy->getTransform()->getPosition()) >= movementIncrement) {
		enemy->rotateTowards(delta, enemy->smoothPath[enemy->currentPathPoint], enemy->rotationSpeed);
		VEC3 dir = enemy->smoothPath[enemy->currentPathPoint] - enemy->getTransform()->getPosition();
		dir.Normalize();
		enemy->deltaMovement += dir * movementIncrement;
		return Stay;
	}
	else if (enemy->currentPathPoint < enemy->smoothPath.size()) {
		enemy->currentPathPoint++;
		return Stay;
	}
}
