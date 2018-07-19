#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class IdleLoop : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	std::string animation = "";
	std::string cancelCondition = "";
public:
	IdleLoop() {}
	IdleLoop(Enemy* enemy, std::string animation, std::string cancelCondition = "");
	int execAction(float delta) override;
	void load(IBehaviorTreeNew* bt, const json& j) override;
};