#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class IdleLoopFlying : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	std::string animation = "";
	std::string cancelCondition = "";
public:
	IdleLoopFlying();
	IdleLoopFlying(Enemy* enemy, std::string animation, std::string cancelCondition = "");
	int execAction(float delta) override;
	void load(IBehaviorTree* bt, const json& j) override;
	void debugInMenu() override;
};