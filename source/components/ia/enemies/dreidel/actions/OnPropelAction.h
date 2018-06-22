#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class OnPropelAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel;
public:
	OnPropelAction(Dreidel* dreidle);
	int execAction(float delta) override;
};