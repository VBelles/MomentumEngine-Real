#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class AttackAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	std::string animation;
	std::string attack;
public:
	AttackAction(Enemy* enemy, std::string animation, std::string attack);
	int execAction(float delta) override;
};