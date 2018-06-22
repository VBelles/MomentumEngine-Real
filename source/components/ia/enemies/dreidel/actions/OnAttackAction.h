#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class OnAttackAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel = nullptr;
public:
	OnAttackAction(Dreidel* dreidel);
	int execAction(float delta) override;
};