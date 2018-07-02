#include "mcv_platform.h"
#include "ReturnToSpawnAction.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_transform.h"
#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

ReturnToSpawnAction::ReturnToSpawnAction(Enemy* enemy, IBehaviorTreeCondition* combatCondition) :
	enemy(enemy),
	combatCondition(combatCondition) {
}

int ReturnToSpawnAction::execAction(float delta) {
	enemy->updateGravity(delta);
	float movementIncrement = enemy->movementSpeed * delta;
	enemy->rotateTowards(delta, enemy->spawnPosition, enemy->rotationSpeed);
	if (combatCondition->testCondition(delta)) {
		return Leave;
	}
	else if (VEC3::DistanceSquared(enemy->spawnPosition, enemy->getTransform()->getPosition()) >= movementIncrement * movementIncrement) {
		enemy->deltaMovement += enemy->getTransform()->getFront() * movementIncrement;
		return Stay;
	}
	else {
		enemy->deltaMovement += enemy->getTransform()->getFront() * VEC3::Distance(enemy->spawnPosition, enemy->getTransform()->getPosition());
		return Leave;
	}
}
