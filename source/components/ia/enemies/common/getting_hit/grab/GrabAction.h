#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class GrabAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
public:
	GrabAction(Enemy* enemy);
	int execAction(float delta) override;
};