#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class DeathAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	std::string animation;
public:
	DeathAction(Enemy* enemy, std::string animation);
	int execAction(float delta) override;
};