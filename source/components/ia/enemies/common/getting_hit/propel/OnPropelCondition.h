#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Enemy;

class OnPropelCondition : public IBehaviorTreeCondition {
private:
	Enemy * enemy;
public:
	OnPropelCondition(Enemy* enemy);
	bool testCondition(float delta) override;
};