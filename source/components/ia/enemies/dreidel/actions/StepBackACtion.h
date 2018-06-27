#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class StepBackAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
public:
	StepBackAction(Enemy* enemy);
	int execAction(float delta) override;
};