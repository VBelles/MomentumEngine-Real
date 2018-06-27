#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class IdleWarAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
public:
	IdleWarAction(Enemy* enemy);
	int execAction(float delta) override;
};