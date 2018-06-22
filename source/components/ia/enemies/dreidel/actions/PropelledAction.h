#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Dreidel;

class PropelledAction : public IBehaviorTreeAction {
private:
	Dreidel* dreidel;
public:
	PropelledAction(Dreidel* dreidle);
	int execAction(float delta) override;
};