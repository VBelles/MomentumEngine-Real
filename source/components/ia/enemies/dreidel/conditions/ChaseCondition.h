#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Dreidel;

class ChaseCondition : public IBehaviorTreeCondition {
private:
	Dreidel* dreidel = nullptr;
public:
	ChaseCondition(Dreidel* dreidel);
	bool testCondition(float delta) override;
};