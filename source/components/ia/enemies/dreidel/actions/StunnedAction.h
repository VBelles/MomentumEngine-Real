#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class StunnedAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
public:
	StunnedAction(Enemy* enemy);
	int execAction(float delta) override;
};