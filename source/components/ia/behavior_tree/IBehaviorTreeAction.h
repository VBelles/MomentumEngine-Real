#pragma once

#include "components/ia/behavior_tree/BTActionFactory.h"

class IBehaviorTreeNew;

class IBehaviorTreeAction {
protected:
	std::string type = "";
public:
	virtual int execAction(float delta) = 0;
	virtual void load(IBehaviorTreeNew* bt, const json& j) {}
	virtual std::string getType();
	virtual void debugInMenu() {}
};