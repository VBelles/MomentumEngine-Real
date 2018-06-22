#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class FalseCondition: public IBehaviorTreeCondition {
public:
	bool testCondition(float delta) override;
};