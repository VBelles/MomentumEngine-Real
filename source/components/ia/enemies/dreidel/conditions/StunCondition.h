#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Dreidel;

class StunCondition : public IBehaviorTreeCondition {
private:
	Dreidel* dreidel;
public:
	StunCondition(Dreidel* dreidle);
	bool testCondition(float delta) override;
};