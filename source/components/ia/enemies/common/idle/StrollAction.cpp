#include "mcv_platform.h"
#include "StrollAction.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_transform.h"

StrollAction::StrollAction(Enemy* enemy) :
	enemy(enemy) {
}

int StrollAction::execAction(float delta) {
	enemy->updateGravity(delta);
	float movementIncrement = enemy->movementSpeed * delta;
	enemy->rotateTowards(delta, enemy->nextWaypoint, enemy->rotationSpeed);
	if (VEC3::DistanceSquared(enemy->nextWaypoint, enemy->getTransform()->getPosition()) >= movementIncrement * movementIncrement) {
		enemy->deltaMovement += enemy->getTransform()->getFront() * movementIncrement;
		return Stay;
	}
	else {
		enemy->deltaMovement += enemy->getTransform()->getFront() * VEC3::Distance(enemy->nextWaypoint, enemy->getTransform()->getPosition());
		return Leave;
	}
}
