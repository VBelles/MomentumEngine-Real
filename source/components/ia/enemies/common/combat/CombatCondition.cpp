#include "mcv_platform.h"
#include "CombatCondition.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_transform.h"

REGISTER_BTCONDITION("CombatCondition", CombatCondition);

CombatCondition::CombatCondition() {
	type = "CombatCondition";
}

CombatCondition::CombatCondition(Enemy* enemy) :
	CombatCondition() {
	this->enemy = enemy;
}

bool CombatCondition::testCondition(float delta) {
	VEC3 playerPosition = enemy->getPlayerTransform()->getPosition();
	float distanceSqrd = VEC3::DistanceSquared(enemy->getTransform()->getPosition(), playerPosition);
	return distanceSqrd <= enemy->smallChaseRadiusSqrd ||
			distanceSqrd <= enemy->fovChaseDistanceSqrd && enemy->getTransform()->isInFov(playerPosition, enemy->chaseFov);
}

void CombatCondition::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}