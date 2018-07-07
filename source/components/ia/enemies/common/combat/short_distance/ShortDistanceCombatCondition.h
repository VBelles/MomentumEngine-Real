#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Enemy;

class ShortDistanceCombatCondition : public IBehaviorTreeCondition {
private:
	Enemy * enemy;
public:
	ShortDistanceCombatCondition(Enemy* enemy);
	bool testCondition(float delta) override;
};