#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class AirborneAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel;
public:
	AirborneAction(Dreidel* dreidle);
	int execAction(float delta) override;
};