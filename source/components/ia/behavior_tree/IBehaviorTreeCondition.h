#pragma once

#include "components/ia/behavior_tree/BTConditionFactory.h"

class IBehaviorTree;

class IBehaviorTreeCondition {
protected:
	std::string type = "";
public:
	virtual bool testCondition(float delta) = 0;
	virtual void load(IBehaviorTree* bt, const json& j) {}
	virtual std::string getType();
	virtual void debugInMenu() {}
};