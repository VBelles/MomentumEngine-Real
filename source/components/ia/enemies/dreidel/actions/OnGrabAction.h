#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class OnGrabAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel = nullptr;
public:
	OnGrabAction(Dreidel* dreidel);
	int execAction(float delta) override;
};