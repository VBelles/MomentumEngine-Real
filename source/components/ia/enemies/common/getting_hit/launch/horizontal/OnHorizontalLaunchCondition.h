#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class OnHorizontalLaunchCondition : public IBehaviorTreeCondition {
private:
	Enemy* enemy;
public:
	OnHorizontalLaunchCondition(Enemy* enemy);
	bool testCondition(float delta) override;
};