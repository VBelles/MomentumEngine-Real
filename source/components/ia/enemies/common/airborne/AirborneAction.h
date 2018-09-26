#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class AirborneAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
public:
	AirborneAction();
	AirborneAction(Enemy* enemy);
	int execAction(float delta) override;
	void load(IBehaviorTree* bt, const json& j) override;
};