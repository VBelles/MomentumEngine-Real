#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Enemy;

class MediumDistanceCombatCondition : public IBehaviorTreeCondition {
private:
	Enemy * enemy;
public:
	MediumDistanceCombatCondition(Enemy* enemy);
	bool testCondition(float delta) override;
};