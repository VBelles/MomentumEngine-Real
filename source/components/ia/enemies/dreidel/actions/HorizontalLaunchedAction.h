#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class HorizontalLaunchedAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel;
public:
	HorizontalLaunchedAction(Dreidel* dreidle);
	int execAction(float delta) override;
};