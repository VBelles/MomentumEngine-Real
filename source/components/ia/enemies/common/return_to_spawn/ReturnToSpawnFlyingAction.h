#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class ReturnToSpawnFlyingAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	std::string cancelCondition = "";
public:
	ReturnToSpawnFlyingAction();
	ReturnToSpawnFlyingAction(Enemy* enemy, std::string cancelCondition = "");
	int execAction(float delta) override;
	void load(IBehaviorTreeNew* bt, const json& j) override;
};