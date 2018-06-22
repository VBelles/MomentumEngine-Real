#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class ReturnToSpawnAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel;
public:
	ReturnToSpawnAction(Dreidel* dreidle);
	int execAction(float delta) override;
};