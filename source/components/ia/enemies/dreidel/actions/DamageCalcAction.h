#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class DamageCalcAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel;
public:
	DamageCalcAction(Dreidel* dreidel);
	int execAction(float delta) override;
};