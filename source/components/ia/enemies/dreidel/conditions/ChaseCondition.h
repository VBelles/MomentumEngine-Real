#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Dreidel;

class ChaseCondition : public IBehaviorTreeCondition {
private:
	Dreidel* dreidel;
public:
	ChaseCondition(Dreidel* dreidle);
	bool testCondition(float delta) override;
};