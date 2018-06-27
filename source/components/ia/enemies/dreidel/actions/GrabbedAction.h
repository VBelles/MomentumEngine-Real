#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class GrabbedAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
public:
	GrabbedAction(Enemy* enemy);
	int execAction(float delta) override;
};