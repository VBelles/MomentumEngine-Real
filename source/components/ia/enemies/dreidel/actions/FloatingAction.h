#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class FloatingAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
public:
	FloatingAction(Enemy* enemy);
	int execAction(float delta) override;
};