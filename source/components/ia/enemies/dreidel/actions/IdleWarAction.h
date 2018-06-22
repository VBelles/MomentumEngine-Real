#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class IdleWarAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel = nullptr;
public:
	IdleWarAction(Dreidel* dreidel);
	int execAction(float delta) override;
};