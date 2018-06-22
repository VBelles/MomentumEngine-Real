#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class OnDeathAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel;
public:
	OnDeathAction(Dreidel* dreidle);
	int execAction(float delta) override;
};