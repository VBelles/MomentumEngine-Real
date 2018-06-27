#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class DeadAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
public:
	DeadAction(Enemy* enemy);
	int execAction(float delta) override;
};