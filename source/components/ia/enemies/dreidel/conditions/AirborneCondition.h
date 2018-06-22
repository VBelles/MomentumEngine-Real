#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Dreidel;

class AirborneCondition : public IBehaviorTreeCondition {
private:
	Dreidel* dreidel = nullptr;
public:
	AirborneCondition(Dreidel* dreidel);
	bool testCondition(float delta) override;
};