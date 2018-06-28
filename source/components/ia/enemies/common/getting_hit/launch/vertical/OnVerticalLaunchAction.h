#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class OnVerticalLaunchAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	std::string animation;
public:
	OnVerticalLaunchAction(Enemy* enemy, std::string animation);
	int execAction(float delta) override;
};