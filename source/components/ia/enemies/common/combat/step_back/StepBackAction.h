#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class StepBackAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	std::string animation = "";
	float speed = 1.f;
public:
	StepBackAction();
	StepBackAction(Enemy* enemy, std::string animation, float speed);
	int execAction(float delta) override;
	void load(IBehaviorTreeNew* bt, const json& j) override;
	void debugInMenu() override;
};