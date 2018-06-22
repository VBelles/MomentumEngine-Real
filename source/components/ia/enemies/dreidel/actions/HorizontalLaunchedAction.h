#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class HorizontalLaunchedAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel = nullptr;
public:
	HorizontalLaunchedAction(Dreidel* dreidel);
	int execAction(float delta) override;
};