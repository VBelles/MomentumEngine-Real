#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class OnStunAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
public:
	OnStunAction(Enemy* enemy);
	int execAction(float delta) override;
};