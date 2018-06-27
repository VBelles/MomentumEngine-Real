#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class OnVerticalLaunchAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
public:
	OnVerticalLaunchAction(Enemy* enemy);
	int execAction(float delta) override;
};