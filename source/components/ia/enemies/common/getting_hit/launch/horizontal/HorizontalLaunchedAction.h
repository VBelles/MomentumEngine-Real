#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class HorizontalLaunchedAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	std::string attack;
public:
	HorizontalLaunchedAction();
	HorizontalLaunchedAction(Enemy* enemy, std::string attack);
	int execAction(float delta) override;
	void load(IBehaviorTreeNew* bt, const json& j) override;
};