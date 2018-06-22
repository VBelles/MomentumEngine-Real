#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class OnGrabAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel;
public:
	OnGrabAction(Dreidel* dreidle);
	int execAction(float delta) override;
};