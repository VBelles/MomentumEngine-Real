#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class OnPropelAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	std::string attack;
public:
	OnPropelAction(Enemy* enemy, std::string attack);
	int execAction(float delta) override;
};