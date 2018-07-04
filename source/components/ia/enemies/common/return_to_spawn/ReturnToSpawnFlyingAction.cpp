#include "mcv_platform.h"
#include "ReturnToSpawnFlyingAction.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_transform.h"
#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

ReturnToSpawnFlyingAction::ReturnToSpawnFlyingAction(Enemy* enemy, IBehaviorTreeCondition* combatCondition) :
	enemy(enemy),
	combatCondition(combatCondition) {
}

int ReturnToSpawnFlyingAction::execAction(float delta) {
	float movementIncrement = enemy->movementSpeed * delta;
	enemy->rotateTowards(delta, enemy->spawnPosition, enemy->rotationSpeed);
	VEC3 direction = enemy->spawnPosition - enemy->getTransform()->getPosition();
	direction.Normalize();
	if (combatCondition->testCondition(delta)) {
		return Leave;
	}
	else if (VEC3::DistanceSquared(enemy->spawnPosition, enemy->getTransform()->getPosition()) >= movementIncrement * movementIncrement) {
		enemy->deltaMovement += direction * movementIncrement;
		return Stay;
	}
	else {
		enemy->deltaMovement += direction * VEC3::Distance(enemy->spawnPosition, enemy->getTransform()->getPosition());
		return Leave;
	}
}
