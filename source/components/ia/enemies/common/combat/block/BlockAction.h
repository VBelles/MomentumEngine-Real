#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class BlockAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
public:
	BlockAction();
	BlockAction(Enemy* enemy);
	int execAction(float delta) override;
	void load(IBehaviorTreeNew* bt, const json& j) override;
};