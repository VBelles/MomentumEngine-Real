#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class FalseCondition: public IBehaviorTreeCondition {
public:
	FalseCondition() {}
	bool testCondition(float delta) override;
};