#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class OnHorizontalLaunchAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel;
public:
	OnHorizontalLaunchAction(Dreidel* dreidle);
	int execAction(float delta) override;
};