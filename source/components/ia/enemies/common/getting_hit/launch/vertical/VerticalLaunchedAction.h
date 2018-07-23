#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class VerticalLaunchedAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	std::string attack = "";
public:
	VerticalLaunchedAction();
	VerticalLaunchedAction(Enemy* enemy, std::string attack);
	int execAction(float delta) override;
	void load(IBehaviorTreeNew* bt, const json& j) override;
	void debugInMenu() override;
};