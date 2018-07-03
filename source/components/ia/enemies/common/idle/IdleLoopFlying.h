#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class IdleLoop : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	std::string animation;
public:
	IdleLoop(Enemy* enemy, std::string animation);
	int execAction(float delta) override;
};