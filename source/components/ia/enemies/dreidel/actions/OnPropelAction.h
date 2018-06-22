#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class OnPropelAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel = nullptr;
public:
	OnPropelAction(Dreidel* dreidel);
	int execAction(float delta) override;
};