#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class OnAttackAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
public:
	OnAttackAction(Enemy* enemy);
	int execAction(float delta) override;
};