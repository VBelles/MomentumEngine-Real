#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class StunnedAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel = nullptr;
public:
	StunnedAction(Dreidel* dreidel);
	int execAction(float delta) override;
};