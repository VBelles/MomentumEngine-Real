#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class IdleAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
public:
	IdleAction(Enemy* enemy);
	int execAction(float delta) override;
};