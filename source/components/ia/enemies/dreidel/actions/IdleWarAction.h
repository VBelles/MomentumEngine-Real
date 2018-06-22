#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class IdleWarAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel;
public:
	IdleWarAction(Dreidel* dreidle);
	int execAction(float delta) override;
};