#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Enemy;

class OnVerticalLaunchCondition : public IBehaviorTreeCondition {
private:
	Enemy* enemy;
public:
	OnVerticalLaunchCondition(Enemy* enemy);
	bool testCondition(float delta) override;
};