#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;
class IBehaviorTreeCondition;

class ReturnToSpawnAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	IBehaviorTreeCondition* cancelCondition = nullptr;

public:
	ReturnToSpawnAction(Enemy* enemy, IBehaviorTreeCondition* cancelCondition = nullptr);
	int execAction(float delta) override;
};