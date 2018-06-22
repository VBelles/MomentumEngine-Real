#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class OnIdleWarAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel = nullptr;
public:
	OnIdleWarAction(Dreidel* dreidel);
	int execAction(float delta) override;
};