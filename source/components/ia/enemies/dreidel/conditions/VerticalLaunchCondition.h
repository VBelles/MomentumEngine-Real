#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Dreidel;

class VerticalLaunchCondition : public IBehaviorTreeCondition {
private:
	Dreidel* dreidel;
public:
	VerticalLaunchCondition(Dreidel* dreidle);
	bool testCondition(float delta) override;
};