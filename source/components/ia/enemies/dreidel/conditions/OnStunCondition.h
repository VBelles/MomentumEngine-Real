#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Dreidel;

class OnStunCondition : public IBehaviorTreeCondition {
private:
	Dreidel* dreidel;
public:
	OnStunCondition(Dreidel* dreidle);
	bool testCondition(float delta) override;
};