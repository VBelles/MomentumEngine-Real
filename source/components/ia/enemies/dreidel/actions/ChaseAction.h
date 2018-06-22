#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class ChaseAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel;
public:
	ChaseAction(Dreidel* dreidle);
	int execAction(float delta) override;
};