#pragma once

#include "components/ia/behavior_tree/BTConditionFactory.h"

class IBehaviorTreeNew;

class IBehaviorTreeCondition {
public:
	virtual bool testCondition(float delta) = 0;
	virtual void load(IBehaviorTreeNew* bt, const json& j) {}
};