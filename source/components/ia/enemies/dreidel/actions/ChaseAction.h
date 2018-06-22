#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class ChaseAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel = nullptr;
public:
	ChaseAction(Dreidel* dreidel);
	int execAction(float delta) override;
};