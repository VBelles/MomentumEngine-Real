#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class IdleAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel = nullptr;
public:
	IdleAction(Dreidel* dreidel);
	int execAction(float delta) override;
};