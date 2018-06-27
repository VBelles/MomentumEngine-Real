#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class ChaseAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
public:
	ChaseAction(Enemy* enemy);
	int execAction(float delta) override;
};