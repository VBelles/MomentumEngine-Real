#include "mcv_platform.h"
#include "StrollAction.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_transform.h"

REGISTER_BTACTION("StrollAction", StrollAction);

StrollAction::StrollAction() {
	type = "StrollAction";
}

StrollAction::StrollAction(Enemy* enemy, std::string cancelCondition) :
	StrollAction() {
	this->enemy = enemy;
	this->cancelCondition = cancelCondition;
}

int StrollAction::execAction(float delta) {
	enemy->updateGravity(delta);
	float movementIncrement = enemy->movementSpeed * delta;
	if (enemy->currentPathPoint >= enemy->smoothPath.size()
		|| !cancelCondition.empty() && enemy->testCondition(cancelCondition, delta)) {
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

void StrollAction::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	cancelCondition = j.value("cancel_condition", cancelCondition);
}

void StrollAction::debugInMenu() {
	ImGui::Text("Cancel condition: %s\n", cancelCondition.c_str());
}