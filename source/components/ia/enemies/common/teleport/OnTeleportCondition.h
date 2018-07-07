#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Enemy;

class OnTeleportCondition : public IBehaviorTreeCondition {
private:
	Enemy * enemy;
public:
	OnTeleportCondition(Enemy* enemy);
	bool testCondition(float delta) override;
};