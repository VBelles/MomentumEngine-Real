#include "mcv_platform.h"
#include "ReturnToSpawnAction.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTACTION("ReturnToSpawnAction", ReturnToSpawnAction);

ReturnToSpawnAction::ReturnToSpawnAction() {
	type = "ReturnToSpawnAction";
}

ReturnToSpawnAction::ReturnToSpawnAction(Enemy* enemy, std::string cancelCondition) :
	ReturnToSpawnAction() {
	this->enemy = enemy;
	this->cancelCondition = cancelCondition;
}

int ReturnToSpawnAction::execAction(float delta) {
	enemy->updateGravity(delta);
	float movementIncrement = enemy->movementSpeed * delta;
	if (enemy->currentPathPoint >= enemy->smoothPath.size()) {
		return Leave;
	}
	else if (!cancelCondition.empty() && enemy->testCondition(cancelCondition, delta)) {
		enemy->getSkeleton()->clear(0.1f);

		return Leave;
	}

	VEC3 nextPoint = enemy->smoothPath[enemy->currentPathPoint];
	VEC3 pos = enemy->getTransform()->getPosition();
	nextPoint.y = 0;
	pos.y = 0;
	enemy->rotateTowards(delta, nextPoint, enemy->rotationSpeed);
	if (VEC3::DistanceSquared(nextPoint, pos) >= movementIncrement * movementIncrement) {
		enemy->deltaMovement += enemy->getTransform()->getFront() * movementIncrement;
	}
	else if (enemy->currentPathPoint < enemy->smoothPath.size()) {
		enemy->deltaMovement += nextPoint - pos;
		enemy->currentPathPoint++;
	}
	return Stay;
}

void ReturnToSpawnAction::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	cancelCondition = j.value("cancel_condition", cancelCondition);
}

void ReturnToSpawnAction::debugInMenu() {
	ImGui::Text("Cancel condition: %s\n", cancelCondition.c_str());
}
