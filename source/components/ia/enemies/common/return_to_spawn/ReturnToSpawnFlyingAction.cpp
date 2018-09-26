#include "mcv_platform.h"
#include "ReturnToSpawnFlyingAction.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_transform.h"

REGISTER_BTACTION("ReturnToSpawnFlyingAction", ReturnToSpawnFlyingAction);

ReturnToSpawnFlyingAction::ReturnToSpawnFlyingAction() {
	type = "ReturnToSpawnFlyingAction";
}

ReturnToSpawnFlyingAction::ReturnToSpawnFlyingAction(Enemy* enemy, std::string cancelCondition) :
	ReturnToSpawnFlyingAction() {
	this->enemy = enemy;
	this->cancelCondition = cancelCondition;
}

int ReturnToSpawnFlyingAction::execAction(float delta) {
	float movementIncrement = enemy->movementSpeed * delta;
	enemy->rotateTowards(delta, enemy->spawnPosition, enemy->rotationSpeed);
	VEC3 direction = enemy->spawnPosition - enemy->getTransform()->getPosition();
	direction.Normalize();
	if (!cancelCondition.empty() && enemy->testCondition(cancelCondition, delta)) {
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

void ReturnToSpawnFlyingAction::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	cancelCondition = j.value("cancel_condition", cancelCondition);
}

void ReturnToSpawnFlyingAction::debugInMenu() {
	ImGui::Text("Cancel condition: %s\n", cancelCondition.c_str());
}