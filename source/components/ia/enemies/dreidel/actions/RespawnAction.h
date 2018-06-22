#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class RespawnAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel;
public:
	RespawnAction(Dreidel* dreidle);
	int execAction(float delta) override;
};