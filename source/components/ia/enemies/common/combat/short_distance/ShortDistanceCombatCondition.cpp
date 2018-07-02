#include "mcv_platform.h"
#include "ShortDistanceCombatCondition.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_transform.h"

ShortDistanceCombatCondition::ShortDistanceCombatCondition(Enemy* enemy) : enemy(enemy) {
}

bool ShortDistanceCombatCondition::testCondition(float delta) {
	VEC3 playerPosition = enemy->getPlayerTransform()->getPosition();
	float distanceSqrd = VEC3::DistanceSquared(enemy->getTransform()->getPosition(), playerPosition);
	return distanceSqrd <= enemy->shortCombatDistanceSqrd;
}
