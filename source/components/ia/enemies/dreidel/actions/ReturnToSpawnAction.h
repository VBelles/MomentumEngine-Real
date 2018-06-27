#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class ReturnToSpawnAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
public:
	ReturnToSpawnAction(Enemy* enemy);
	int execAction(float delta) override;
};