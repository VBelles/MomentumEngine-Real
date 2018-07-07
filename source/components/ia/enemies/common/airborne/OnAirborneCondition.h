#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Enemy;

class OnAirborneCondition : public IBehaviorTreeCondition {
private:
	Enemy * enemy;
public:
	OnAirborneCondition(Enemy* enemy);
	bool testCondition(float delta) override;
};