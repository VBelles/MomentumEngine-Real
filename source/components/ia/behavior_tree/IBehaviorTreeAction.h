#pragma once

#include "components/ia/behavior_tree/BTActionFactory.h"

class IBehaviorTreeNew;

class IBehaviorTreeAction {
public:
	virtual int execAction(float delta) = 0;
	virtual void load(IBehaviorTreeNew* bt, const json& j) {}
};