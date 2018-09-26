#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Enemy;

class BlockCondition : public IBehaviorTreeCondition {
private:
	Enemy* enemy = nullptr;
public:
	BlockCondition();
	BlockCondition(Enemy* enemy);
	bool testCondition(float delta) override;
	void load(IBehaviorTree* bt, const json& j) override;
};
