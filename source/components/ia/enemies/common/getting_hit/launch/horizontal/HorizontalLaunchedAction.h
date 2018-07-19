#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class HorizontalLaunchedAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
public:
	HorizontalLaunchedAction() {}
	HorizontalLaunchedAction(Enemy* enemy);
	int execAction(float delta) override;
	void load(IBehaviorTreeNew* bt, const json& j) override;
};