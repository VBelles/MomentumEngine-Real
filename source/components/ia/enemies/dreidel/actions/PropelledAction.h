#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class PropelledAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel = nullptr;
public:
	PropelledAction(Dreidel* dreidel);
	int execAction(float delta) override;
};