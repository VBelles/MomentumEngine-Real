#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Dreidel;

class DeadCondition : public IBehaviorTreeCondition {
private:
	Dreidel* dreidel = nullptr;
public:
	DeadCondition(Dreidel* dreidel);
	bool testCondition(float delta) override;
};