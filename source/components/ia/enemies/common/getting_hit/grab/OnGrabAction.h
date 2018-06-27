#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class OnGrabAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
public:
	OnGrabAction(Enemy* enemy);
	int execAction(float delta) override;
};