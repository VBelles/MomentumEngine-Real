#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class ReturnToSpawnAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel = nullptr;
public:
	ReturnToSpawnAction(Dreidel* dreidel);
	int execAction(float delta) override;
};