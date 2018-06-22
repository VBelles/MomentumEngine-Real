#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class AttackAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel = nullptr;
public:
	AttackAction(Dreidel* dreidel);
	int execAction(float delta) override;
};