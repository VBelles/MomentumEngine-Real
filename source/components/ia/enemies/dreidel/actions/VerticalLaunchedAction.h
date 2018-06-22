#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class VerticalLaunchedAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel;
public:
	VerticalLaunchedAction(Dreidel* dreidle);
	int execAction(float delta) override;
};