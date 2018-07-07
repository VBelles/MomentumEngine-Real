#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class OnIdleWarAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	std::string animation;
public:
	OnIdleWarAction(Enemy* enemy, std::string animation);
	int execAction(float delta) override;
};