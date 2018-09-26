#include "mcv_platform.h"
#include "ChaseAction.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_transform.h"

REGISTER_BTACTION("ChaseAction", ChaseAction);

ChaseAction::ChaseAction() {
	type = "ChaseAction";
}

ChaseAction::ChaseAction(Enemy* enemy) :
	ChaseAction() {
	this->enemy = enemy;
}

int ChaseAction::execAction(float delta) {
	enemy->updateGravity(delta);
	VEC3 playerPosition = enemy->getPlayerTransform()->getPosition();
	float movementIncrement = enemy->movementSpeed * delta;
	enemy->rotateTowards(delta, playerPosition, enemy->rotationSpeed);

	if (VEC3::DistanceSquared(playerPosition, enemy->getTransform()->getPosition()) >= movementIncrement * movementIncrement) {
		VEC3 deltaMove = movementIncrement * enemy->getTransform()->getFront();
		if (enemy->navMeshQuery) {
			VEC3 pos = enemy->getTransform()->getPosition();
			if (enemy->navMeshQuery->existsConnection(pos, pos + deltaMove)
				&& !enemy->navMeshQuery->raycast2D(pos, pos + deltaMove)) {
				enemy->deltaMovement += deltaMove;
			}
		}
		else {
			enemy->deltaMovement += deltaMove;
		}
	}
	else {
		VEC3 deltaMove = enemy->getTransform()->getFront() * VEC3::Distance(playerPosition, enemy->getTransform()->getPosition());
		if (enemy->navMeshQuery) {
			VEC3 pos = enemy->getTransform()->getPosition();
			if (enemy->navMeshQuery->existsConnection(pos, pos + deltaMove)
				&& !enemy->navMeshQuery->raycast2D(pos, pos + deltaMove)) {
				enemy->deltaMovement += deltaMove;
			}
		}
		else {
			enemy->deltaMovement += deltaMove;
		}
	}

	return Leave;
}

void ChaseAction::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}