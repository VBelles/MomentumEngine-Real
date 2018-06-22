#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class OnVerticalLaunchAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel = nullptr;
public:
	OnVerticalLaunchAction(Dreidel* dreidel);
	int execAction(float delta) override;
};