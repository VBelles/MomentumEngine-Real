#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class OnIdleWarAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel;
public:
	OnIdleWarAction(Dreidel* dreidle);
	int execAction(float delta) override;
};