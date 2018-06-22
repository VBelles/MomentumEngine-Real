#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Dreidel;

class GrabCondition : public IBehaviorTreeCondition {
private:
	Dreidel* dreidel = nullptr;
public:
	GrabCondition(Dreidel* dreidel);
	bool testCondition(float delta) override;
};