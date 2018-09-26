#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Enemy;

class OnBlockingBreakCondition : public IBehaviorTreeCondition {
private:
	Enemy* enemy = nullptr;
public:
	OnBlockingBreakCondition();
	OnBlockingBreakCondition(Enemy* enemy);
	bool testCondition(float delta) override;
	void load(IBehaviorTree* bt, const json& j) override;
};