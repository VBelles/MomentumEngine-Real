#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Enemy;

class OnHorizontalLaunchCondition : public IBehaviorTreeCondition {
private:
	Enemy* enemy;
public:
	OnHorizontalLaunchCondition(Enemy* enemy);
	bool testCondition(float delta) override;
};