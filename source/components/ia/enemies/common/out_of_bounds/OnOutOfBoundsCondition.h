#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Enemy;

class OnOutOfBoundsCondition : public IBehaviorTreeCondition {
private:
	Enemy* enemy = nullptr;
public:
	OnOutOfBoundsCondition();
	OnOutOfBoundsCondition(Enemy* enemy);
	bool testCondition(float delta) override;
	void load(IBehaviorTree* bt, const json& j) override;
};
