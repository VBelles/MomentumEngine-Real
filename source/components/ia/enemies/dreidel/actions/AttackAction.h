#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class AttackAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
public:
	AttackAction(Enemy* enemy);
	int execAction(float delta) override;
};