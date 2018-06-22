#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class OnStunAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel;
public:
	OnStunAction(Dreidel* dreidle);
	int execAction(float delta) override;
};