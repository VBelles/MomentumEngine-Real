#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class AttackAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel;
public:
	AttackAction(Dreidel* dreidle);
	int execAction(float delta) override;
};