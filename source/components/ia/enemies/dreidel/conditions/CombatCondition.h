#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Dreidel;

class CombatCondition : public IBehaviorTreeCondition {
private:
	Dreidel* dreidel;
public:
	CombatCondition(Dreidel* dreidle);
	bool testCondition(float delta) override;
};