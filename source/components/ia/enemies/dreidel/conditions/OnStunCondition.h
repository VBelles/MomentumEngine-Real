#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Dreidel;

class OnStunCondition : public IBehaviorTreeCondition {
private:
	Dreidel* dreidel = nullptr;
public:
	OnStunCondition(Dreidel* dreidel);
	bool testCondition(float delta) override;
};