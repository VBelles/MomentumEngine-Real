#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class PropelledAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
public:
	PropelledAction(Enemy* enemy);
	int execAction(float delta) override;
};