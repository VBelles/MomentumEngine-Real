#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Enemy;

class LongDistanceCombatCondition : public IBehaviorTreeCondition {
private:
	Enemy * enemy;
public:
	LongDistanceCombatCondition(Enemy* enemy);
	bool testCondition(float delta) override;
};