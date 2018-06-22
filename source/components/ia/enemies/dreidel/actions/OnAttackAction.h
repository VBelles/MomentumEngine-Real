#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class OnAttackAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel;
public:
	OnAttackAction(Dreidel* dreidle);
	int execAction(float delta) override;
};