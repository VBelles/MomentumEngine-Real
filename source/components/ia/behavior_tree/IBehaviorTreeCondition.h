#pragma once

#include "components/ia/behavior_tree/BTConditionFactory.h"

class IBehaviorTreeCondition {
public:
	virtual bool testCondition(float delta) = 0;
};