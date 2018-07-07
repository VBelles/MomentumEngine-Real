#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Enemy;

class OnDeathCondition : public IBehaviorTreeCondition {
private:
	Enemy * enemy;
public:
	OnDeathCondition(Enemy* enemy);
	bool testCondition(float delta) override;
};