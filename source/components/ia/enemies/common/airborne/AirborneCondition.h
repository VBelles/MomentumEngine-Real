#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class AirborneCondition : public IBehaviorTreeCondition {
private:
	Enemy * enemy;
public:
	AirborneCondition(Enemy* enemy);
	bool testCondition(float delta) override;
};