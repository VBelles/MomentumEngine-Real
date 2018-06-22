#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Dreidel;

class AirborneCondition : public IBehaviorTreeCondition {
private:
	Dreidel* dreidel;
public:
	AirborneCondition(Dreidel* dreidle);
	bool testCondition(float delta) override;
};