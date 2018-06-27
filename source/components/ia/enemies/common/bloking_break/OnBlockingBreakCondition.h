#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class OnBlockingBreakCondition : public IBehaviorTreeCondition {
private:
	Enemy * enemy;
public:
	OnBlockingBreakCondition(Enemy* enemy);
	bool testCondition(float delta) override;
};