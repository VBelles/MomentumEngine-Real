#pragma once

#include "components/ia/behavior_tree/BTActionFactory.h"

class IBehaviorTreeAction {
public:
	IBehaviorTreeAction() {}
	virtual int execAction(float delta) = 0;
};