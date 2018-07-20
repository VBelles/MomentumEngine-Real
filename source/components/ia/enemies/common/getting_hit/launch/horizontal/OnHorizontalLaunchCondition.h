#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Enemy;

class OnHorizontalLaunchCondition : public IBehaviorTreeCondition {
private:
	Enemy* enemy = nullptr;
public:
	OnHorizontalLaunchCondition();
	OnHorizontalLaunchCondition(Enemy* enemy);
	bool testCondition(float delta) override;
	void load(IBehaviorTreeNew* bt, const json& j) override;
};