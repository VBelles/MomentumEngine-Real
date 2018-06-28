#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Enemy;

class OnStunCondition : public IBehaviorTreeCondition {
private:
	Enemy * enemy;
public:
	OnStunCondition(Enemy* enemy);
	bool testCondition(float delta) override;
};