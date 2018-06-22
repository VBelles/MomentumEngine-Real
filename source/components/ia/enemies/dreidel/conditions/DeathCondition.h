#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Dreidel;

class DeathCondition: public IBehaviorTreeCondition {
private:
	Dreidel* dreidel = nullptr;
public:
	DeathCondition(Dreidel* dreidel);
	bool testCondition(float delta) override;
};