#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class StepBackAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel;
public:
	StepBackAction(Dreidel* dreidle);
	int execAction(float delta) override;
};