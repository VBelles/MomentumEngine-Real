#include "mcv_platform.h"
#include "CombatCondition.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_transform.h"

CombatCondition::CombatCondition(Enemy* enemy) : enemy(enemy) {
}

bool CombatCondition::testCondition(float delta) {
	VEC3 playerPosition = enemy->getPlayerTransform()->getPosition();
	float distanceSqrd = VEC3::DistanceSquared(enemy->getTransform()->getPosition(), playerPosition);
	return distanceSqrd <= enemy->smallChaseRadiusSqrd ||
			distanceSqrd <= enemy->fovChaseDistanceSqrd && enemy->getTransform()->isInFov(playerPosition, enemy->chaseFov);
}
