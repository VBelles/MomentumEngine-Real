#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class OnIdleWarAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
public:
	OnIdleWarAction(Enemy* enemy);
	int execAction(float delta) override;
};