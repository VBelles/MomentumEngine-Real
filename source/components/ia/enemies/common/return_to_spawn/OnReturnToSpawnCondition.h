#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Enemy;

class OnReturnToSpawnCondition : public IBehaviorTreeCondition {
private:
	Enemy* enemy = nullptr;
public:
	OnReturnToSpawnCondition() {}
	OnReturnToSpawnCondition(Enemy* enemy);
	bool testCondition(float delta) override;
	void load(IBehaviorTreeNew* bt, const json& j) override;
};