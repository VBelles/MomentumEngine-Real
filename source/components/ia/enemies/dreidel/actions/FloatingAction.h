#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class FloatingAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel;
public:
	FloatingAction(Dreidel* dreidle);
	int execAction(float delta) override;
};