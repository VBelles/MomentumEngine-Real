#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Dreidel;

class DeadCondition : public IBehaviorTreeCondition {
private:
	Dreidel* dreidel;
public:
	DeadCondition(Dreidel* dreidle);
	bool testCondition(float delta) override;
};