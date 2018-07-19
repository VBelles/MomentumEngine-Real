#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Enemy;

class ShortDistanceCombatCondition : public IBehaviorTreeCondition {
private:
	Enemy* enemy = nullptr;
public:
	ShortDistanceCombatCondition() {}
	ShortDistanceCombatCondition(Enemy* enemy);
	bool testCondition(float delta) override;
	void load(IBehaviorTreeNew* bt, const json& j) override;
};