#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Dreidel;

class CombatCondition : public IBehaviorTreeCondition {
private:
	Dreidel* dreidel = nullptr;
public:
	CombatCondition(Dreidel* dreidel);
	bool testCondition(float delta) override;
};