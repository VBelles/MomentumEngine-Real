#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class RespawnAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
public:
	RespawnAction(Enemy* enemy);
	int execAction(float delta) override;
};