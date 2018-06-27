#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class OnHorizontalLaunchAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
public:
	OnHorizontalLaunchAction(Enemy* enemy);
	int execAction(float delta) override;
};