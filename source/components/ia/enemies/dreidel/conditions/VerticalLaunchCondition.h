#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Dreidel;

class VerticalLaunchCondition : public IBehaviorTreeCondition {
private:
	Dreidel* dreidel = nullptr;
public:
	VerticalLaunchCondition(Dreidel* dreidel);
	bool testCondition(float delta) override;
};