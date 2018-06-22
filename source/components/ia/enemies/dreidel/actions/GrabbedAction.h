#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class GrabbedAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel;
public:
	GrabbedAction(Dreidel* dreidle);
	int execAction(float delta) override;
};