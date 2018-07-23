#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class OnHorizontalLaunchAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	std::string animation;
	std::string attack;
public:
	OnHorizontalLaunchAction();
	OnHorizontalLaunchAction(Enemy* enemy, std::string animation, std::string attack);
	int execAction(float delta) override;
	void load(IBehaviorTreeNew* bt, const json& j) override;
};