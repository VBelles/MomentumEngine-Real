#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Enemy;

class AttackCoolDownCondition : public IBehaviorTreeCondition {
private:
	Enemy * enemy;
public:
	AttackCoolDownCondition(Enemy* enemy);
	bool testCondition(float delta) override;
};