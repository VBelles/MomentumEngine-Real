#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Dreidel;

class DeathCondition: public IBehaviorTreeCondition {
private:
	Dreidel* dreidel;
public:
	DeathCondition(Dreidel* dreidle);
	bool testCondition(float delta) override;
};