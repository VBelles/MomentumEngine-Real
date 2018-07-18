#include "mcv_platform.h"
#include "ChaseAction.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_transform.h"

REGISTER_BTAction("ChaseAction", ChaseAction);

ChaseAction::ChaseAction(Enemy* enemy) :
	enemy(enemy) {
}

int ChaseAction::execAction(float delta) {
	enemy->updateGravity(delta);
	VEC3 playerPosition = enemy->getPlayerTransform()->getPosition();
	float movementIncrement = enemy->movementSpeed * delta;
	enemy->rotateTowards(delta, playerPosition, enemy->rotationSpeed);
	if (VEC3::DistanceSquared(playerPosition, enemy->getTransform()->getPosition()) >= movementIncrement * movementIncrement) {
		enemy->deltaMovement += enemy->getTransform()->getFront() * movementIncrement;
	}
	else {
		enemy->deltaMovement += enemy->getTransform()->getFront() * VEC3::Distance(playerPosition, enemy->getTransform()->getPosition());
	}
	return Leave;
}

void ChaseAction::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}