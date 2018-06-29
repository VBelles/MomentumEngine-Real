#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class StrollAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
public:
	StrollAction(Enemy* enemy);
	int execAction(float delta) override;
};