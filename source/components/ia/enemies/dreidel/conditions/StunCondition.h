#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Dreidel;

class StunCondition : public IBehaviorTreeCondition {
private:
	Dreidel* dreidel = nullptr;
public:
	StunCondition(Dreidel* dreidel);
	bool testCondition(float delta) override;
};