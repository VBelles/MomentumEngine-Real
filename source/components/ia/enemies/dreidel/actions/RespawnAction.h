#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class RespawnAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel = nullptr;
public:
	RespawnAction(Dreidel* dreidel);
	int execAction(float delta) override;
};