#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Dreidel;

class PropelCondition : public IBehaviorTreeCondition {
private:
	Dreidel* dreidel = nullptr;
public:
	PropelCondition(Dreidel* dreidel);
	bool testCondition(float delta) override;
};