#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class OnVerticalLaunchAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	std::string animation;
	std::string attack;
public:
	OnVerticalLaunchAction(Enemy* enemy, std::string animation, std::string attack);
	int execAction(float delta) override;
};