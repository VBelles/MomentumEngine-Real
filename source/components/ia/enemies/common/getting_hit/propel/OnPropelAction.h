#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class OnPropelAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
public:
	OnPropelAction(Enemy* enemy);
	int execAction(float delta) override;
};