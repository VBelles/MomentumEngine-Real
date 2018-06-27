#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class OnDeathAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
public:
	OnDeathAction(Enemy* enemy);
	int execAction(float delta) override;
};