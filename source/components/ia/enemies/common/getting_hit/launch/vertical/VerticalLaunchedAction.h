#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class VerticalLaunchedAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	std::string attack;
public:
	VerticalLaunchedAction(Enemy* enemy);
	int execAction(float delta) override;
};