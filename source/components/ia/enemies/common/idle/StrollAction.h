#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;
class IBehaviorTreeCondition;

class StrollAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	IBehaviorTreeCondition* cancelCondition = nullptr;

public:
	StrollAction(Enemy* enemy, IBehaviorTreeCondition* cancelCondition = nullptr);
	int execAction(float delta) override;
};