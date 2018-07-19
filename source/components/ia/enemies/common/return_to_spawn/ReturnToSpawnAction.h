#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class ReturnToSpawnAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	std::string cancelCondition = "";
public:
	ReturnToSpawnAction() {}
	ReturnToSpawnAction(Enemy* enemy, std::string cancelCondition = "");
	int execAction(float delta) override;
	void load(IBehaviorTreeNew* bt, const json& j) override;
};