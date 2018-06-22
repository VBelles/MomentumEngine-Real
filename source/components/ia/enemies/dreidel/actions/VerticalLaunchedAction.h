#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class VerticalLaunchedAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel = nullptr;
public:
	VerticalLaunchedAction(Dreidel* dreidel);
	int execAction(float delta) override;
};