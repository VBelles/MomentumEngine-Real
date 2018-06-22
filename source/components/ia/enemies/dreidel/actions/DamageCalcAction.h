#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class DamageCalcAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel;
public:
	DamageCalcAction(Dreidel* dreidle);
	int execAction(float delta) override;
};