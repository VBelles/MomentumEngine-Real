#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;
class IBehaviorTreeCondition;

class ReturnToSpawnAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	IBehaviorTreeCondition* combatCondition;
public:
	ReturnToSpawnAction(Enemy* enemy, IBehaviorTreeCondition* combatCondition);
	int execAction(float delta) override;
};