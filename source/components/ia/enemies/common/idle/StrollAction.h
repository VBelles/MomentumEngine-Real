#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;
class IBehaviorTreeCondition;

class StrollAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	IBehaviorTreeCondition* combatCondition;
public:
	StrollAction(Enemy* enemy, IBehaviorTreeCondition* combatCondition);
	int execAction(float delta) override;
};