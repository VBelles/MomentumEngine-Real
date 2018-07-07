#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Enemy;

class BlockCondition : public IBehaviorTreeCondition {
private:
	Enemy * enemy;
public:
	BlockCondition(Enemy* enemy);
	bool testCondition(float delta) override;
};