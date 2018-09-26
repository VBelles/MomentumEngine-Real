#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Enemy;

class OnAirborneCondition : public IBehaviorTreeCondition {
private:
	Enemy* enemy = nullptr;
public:
	OnAirborneCondition();
	OnAirborneCondition(Enemy* enemy);
	bool testCondition(float delta) override;
	void load(IBehaviorTree* bt, const json& j) override;
};