#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class StepBackAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel = nullptr;
public:
	StepBackAction(Dreidel* dreidel);
	int execAction(float delta) override;
};