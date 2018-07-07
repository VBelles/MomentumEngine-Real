#include "mcv_platform.h"
#include "ReturnToSpawnAction.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_transform.h"
#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

ReturnToSpawnAction::ReturnToSpawnAction(Enemy* enemy, IBehaviorTreeCondition* cancelCondition) :
	enemy(enemy),
	cancelCondition(cancelCondition) {
}

int ReturnToSpawnAction::execAction(float delta) {
	enemy->updateGravity(delta);
	float movementIncrement = enemy->movementSpeed * delta;
	if (enemy->currentPathPoint >= enemy->smoothPath.size() || (cancelCondition && cancelCondition->testCondition(delta))) {
		return Leave;
	}
	else if (VEC3::DistanceSquared(enemy->smoothPath[enemy->currentPathPoint], enemy->getTransform()->getPosition()) >= movementIncrement * movementIncrement) {
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
