#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class DeadAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel = nullptr;
public:
	DeadAction(Dreidel* dreidel);
	int execAction(float delta) override;
};