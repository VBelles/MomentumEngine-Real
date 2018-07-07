#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class OnReturnToSpawnAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	std::string animation;
public:
	OnReturnToSpawnAction(Enemy* enemy, std::string animation);
	int execAction(float delta) override;
};