#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class StunnedAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel;
public:
	StunnedAction(Dreidel* dreidle);
	int execAction(float delta) override;
};