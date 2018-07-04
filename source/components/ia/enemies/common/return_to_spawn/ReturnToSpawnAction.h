#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;
class IBehaviorTreeCondition;

class ReturnToSpawnAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	IBehaviorTreeCondition* combatCondition = nullptr;
	float spawnRadiusDistanceSquared = 1.f;

public:
	ReturnToSpawnAction(Enemy* enemy, IBehaviorTreeCondition* combatCondition);
	int execAction(float delta) override;
};