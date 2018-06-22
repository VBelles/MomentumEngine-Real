#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class DeadAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel;
public:
	DeadAction(Dreidel* dreidle);
	int execAction(float delta) override;
};