#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Enemy;

class AirborneCondition : public IBehaviorTreeCondition {
private:
	Enemy * enemy = nullptr;
public:
	AirborneCondition() {}
	AirborneCondition(Enemy* enemy);
	bool testCondition(float delta) override;
};