#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Dreidel;

class GrabCondition : public IBehaviorTreeCondition {
private:
	Dreidel* dreidel;
public:
	GrabCondition(Dreidel* dreidle);
	bool testCondition(float delta) override;
};