#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class IdleAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel;
public:
	IdleAction(Dreidel* dreidle);
	int execAction(float delta) override;
};