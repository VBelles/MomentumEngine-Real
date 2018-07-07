#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;
class IBehaviorTreeCondition;

class IdleActionFlying : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	std::string animation;
	IBehaviorTreeCondition* cancelCondition = nullptr;

public:
	IdleActionFlying(Enemy* enemy, std::string animation, IBehaviorTreeCondition* cancelCondition = nullptr);
	int execAction(float delta) override;
};