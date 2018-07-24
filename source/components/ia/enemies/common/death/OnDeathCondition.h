#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Enemy;

class OnDeathCondition : public IBehaviorTreeCondition {
private:
	Enemy* enemy = nullptr;
public:
	OnDeathCondition();
	OnDeathCondition(Enemy* enemy);
	bool testCondition(float delta) override;
	void load(IBehaviorTreeNew* bt, const json& j) override;
};