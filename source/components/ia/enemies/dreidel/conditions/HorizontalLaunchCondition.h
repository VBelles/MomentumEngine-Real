#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Dreidel;

class HorizontalLaunchCondition : public IBehaviorTreeCondition {
private:
	Dreidel* dreidel;
public:
	HorizontalLaunchCondition(Dreidel* dreidle);
	bool testCondition(float delta) override;
};