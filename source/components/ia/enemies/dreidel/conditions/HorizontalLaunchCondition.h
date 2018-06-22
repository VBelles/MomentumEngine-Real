#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Dreidel;

class HorizontalLaunchCondition : public IBehaviorTreeCondition {
private:
	Dreidel* dreidel = nullptr;
public:
	HorizontalLaunchCondition(Dreidel* dreidel);
	bool testCondition(float delta) override;
};