#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Enemy;

class CombatCondition : public IBehaviorTreeCondition {
private:
	Enemy * enemy;
public:
	CombatCondition(Enemy* enemy);
	bool testCondition(float delta) override;
};