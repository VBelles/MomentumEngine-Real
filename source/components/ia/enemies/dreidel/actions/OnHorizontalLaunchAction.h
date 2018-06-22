#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class OnHorizontalLaunchAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel = nullptr;
public:
	OnHorizontalLaunchAction(Dreidel* dreidel);
	int execAction(float delta) override;
};