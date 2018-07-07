#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class OnTackleAttackAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	std::string animation;
	std::string attack;
public:
	OnTackleAttackAction(Enemy* enemy, std::string animation, std::string attack);
	int execAction(float delta) override;
};