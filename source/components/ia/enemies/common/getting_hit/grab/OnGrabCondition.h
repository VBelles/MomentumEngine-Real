#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Enemy;

class OnGrabCondition : public IBehaviorTreeCondition {
private:
	Enemy * enemy;
public:
	OnGrabCondition(Enemy* enemy);
	bool testCondition(float delta) override;
};