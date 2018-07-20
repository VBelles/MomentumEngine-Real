#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class StrollAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	std::string cancelCondition = "";
public:
	StrollAction();
	StrollAction(Enemy* enemy, std::string cancelCondition = "");
	int execAction(float delta) override;
	void load(IBehaviorTreeNew* bt, const json& j) override;
};