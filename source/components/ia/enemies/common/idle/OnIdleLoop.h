#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class OnIdleLoop : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	std::string animation = "";
public:
	OnIdleLoop();
	OnIdleLoop(Enemy* enemy, std::string animation);
	int execAction(float delta) override;
	void load(IBehaviorTree* bt, const json& j) override;
	void debugInMenu() override;
};
