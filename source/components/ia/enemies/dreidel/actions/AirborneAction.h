#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class AirborneAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel = nullptr;
public:
	AirborneAction(Dreidel* dreidel);
	int execAction(float delta) override;
};