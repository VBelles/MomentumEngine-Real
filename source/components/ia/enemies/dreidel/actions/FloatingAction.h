#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class FloatingAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel = nullptr;
public:
	FloatingAction(Dreidel* dreidel);
	int execAction(float delta) override;
};