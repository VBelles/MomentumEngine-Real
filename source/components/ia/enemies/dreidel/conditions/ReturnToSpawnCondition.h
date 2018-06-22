#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Dreidel;

class ReturnToSpawnCondition : public IBehaviorTreeCondition {
private:
	Dreidel* dreidel;
public:
	ReturnToSpawnCondition(Dreidel* dreidle);
	bool testCondition(float delta) override;
};