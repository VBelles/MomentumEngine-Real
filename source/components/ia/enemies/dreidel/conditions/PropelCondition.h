#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Dreidel;

class PropelCondition : public IBehaviorTreeCondition {
private:
	Dreidel* dreidel;
public:
	PropelCondition(Dreidel* dreidle);
	bool testCondition(float delta) override;
};