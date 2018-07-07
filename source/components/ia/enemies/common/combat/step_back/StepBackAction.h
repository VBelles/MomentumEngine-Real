#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class StepBackAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	std::string animation;
	float speed;
public:
	StepBackAction(Enemy* enemy, std::string animation, float speed);
	int execAction(float delta) override;
};