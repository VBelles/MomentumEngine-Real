#include "mcv_platform.h"
#include "StrollAction.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_transform.h"
#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

StrollAction::StrollAction(Enemy* enemy, IBehaviorTreeCondition* cancelCondition) :
	enemy(enemy),
	cancelCondition(cancelCondition) {
}

int StrollAction::execAction(float delta) {
	enemy->updateGravity(delta);
	float movementIncrement = enemy->movementSpeed * delta;
	if (enemy->currentPathPoint >= enemy->smoothPath.size() || cancelCondition && (cancelCondition->testCondition(delta))) {
		return Leave;
	}
	VEC3 nextPoint = enemy->smoothPath[enemy->currentPathPoint];
	VEC3 pos = enemy->getTransform()->getPosition();
	nextPoint.y = 0;
	pos.y = 0;
	enemy->rotateTowards(delta, nextPoint, enemy->rotationSpeed);
	if (VEC3::DistanceSquared(nextPoint, pos) >= movementIncrement * movementIncrement) {
		enemy->deltaMovement += enemy->getTransform()->getFront() * movementIncrement;
		return Stay;
	}
	else if (enemy->currentPathPoint < enemy->smoothPath.size()) {
		enemy->deltaMovement += nextPoint - pos;
		enemy->currentPathPoint++;
		return Stay;
	}
}
