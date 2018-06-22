#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Dreidel;

class StepBackCondition : public IBehaviorTreeCondition {
private:
	Dreidel* dreidel;
public:
	StepBackCondition(Dreidel* dreidle);
	bool testCondition(float delta) override;
};