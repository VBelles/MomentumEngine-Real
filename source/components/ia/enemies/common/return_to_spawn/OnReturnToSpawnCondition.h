#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Enemy;

class OnReturnToSpawnCondition : public IBehaviorTreeCondition {
private:
	Enemy * enemy;
public:
	OnReturnToSpawnCondition(Enemy* enemy);
	bool testCondition(float delta) override;
};