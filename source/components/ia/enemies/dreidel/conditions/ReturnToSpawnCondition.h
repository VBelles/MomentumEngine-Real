#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Dreidel;

class ReturnToSpawnCondition : public IBehaviorTreeCondition {
private:
	Dreidel* dreidel = nullptr;
public:
	ReturnToSpawnCondition(Dreidel* dreidel);
	bool testCondition(float delta) override;
};