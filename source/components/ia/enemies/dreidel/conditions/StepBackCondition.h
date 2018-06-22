#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Dreidel;

class StepBackCondition : public IBehaviorTreeCondition {
private:
	Dreidel* dreidel = nullptr;
public:
	StepBackCondition(Dreidel* dreidel);
	bool testCondition(float delta) override;
};