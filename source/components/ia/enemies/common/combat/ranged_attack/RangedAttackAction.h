#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;
class IBehaviorTreeCondition;

class RangedAttackAction : public IBehaviorTreeAction {
private:
	Enemy * enemy = nullptr;
	IBehaviorTreeCondition* cancelCondition = nullptr;
	std::string animation;
	std::string attack;
	bool attackLaunched = false;
public:
	RangedAttackAction(Enemy* enemy, std::string animation, std::string attack, IBehaviorTreeCondition* cancelCondition = nullptr);
	int execAction(float delta) override;
};