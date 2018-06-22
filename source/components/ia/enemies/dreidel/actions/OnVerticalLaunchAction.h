#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class OnVerticalLaunchAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel;
public:
	OnVerticalLaunchAction(Dreidel* dreidle);
	int execAction(float delta) override;
};