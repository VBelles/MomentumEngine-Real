#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class GrabbedAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel = nullptr;
public:
	GrabbedAction(Dreidel* dreidel);
	int execAction(float delta) override;
};