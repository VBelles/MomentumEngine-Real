#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class OnStunAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel = nullptr;
public:
	OnStunAction(Dreidel* dreidel);
	int execAction(float delta) override;
};