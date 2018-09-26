#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Enemy;

class OnSpawnCondition : public IBehaviorTreeCondition {
private:
	Enemy* enemy = nullptr;
public:
	OnSpawnCondition();
	OnSpawnCondition(Enemy* enemy);
	bool testCondition(float delta) override;
	void load(IBehaviorTree* bt, const json& j) override;
};