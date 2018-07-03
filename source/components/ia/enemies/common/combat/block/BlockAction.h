#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class BlockAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
public:
	BlockAction(Enemy* enemy);
	int execAction(float delta) override;
};