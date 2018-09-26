#pragma once

#include "components/ia/behavior_tree/BTActionFactory.h"

class IBehaviorTree;

class IBehaviorTreeAction {
protected:
	std::string type = "";
public:
	virtual int execAction(float delta) = 0;
	virtual void load(IBehaviorTree* bt, const json& j) {}
	virtual std::string getType();
	virtual void debugInMenu() {}
};