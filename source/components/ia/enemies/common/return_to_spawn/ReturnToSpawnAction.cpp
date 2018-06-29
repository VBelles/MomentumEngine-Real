#include "mcv_platform.h"
#include "ReturnToSpawnAction.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_transform.h"


ReturnToSpawnAction::ReturnToSpawnAction(Enemy* enemy) :
	enemy(enemy) {
}

int ReturnToSpawnAction::execAction(float delta) {
	enemy->updateGravity(delta);
	float movementIncrement = enemy->movementSpeed * delta;
	enemy->rotateTowards(delta, enemy->spawnPosition, enemy->rotationSpeed);
	if (VEC3::DistanceSquared(enemy->spawnPosition, enemy->getTransform()->getPosition()) >= movementIncrement * movementIncrement) {
		enemy->deltaMovement += enemy->getTransform()->getFront() * movementIncrement;
		return Stay;
	}
	else {
		enemy->deltaMovement += enemy->getTransform()->getFront() * VEC3::Distance(enemy->spawnPosition, enemy->getTransform()->getPosition());
		return Leave;
	}
}
