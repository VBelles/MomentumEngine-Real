#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Enemy;

class MediumDistanceCombatCondition : public IBehaviorTreeCondition {
private:
	Enemy* enemy = nullptr;
public:
	MediumDistanceCombatCondition() {}
	MediumDistanceCombatCondition(Enemy* enemy);
	bool testCondition(float delta) override;
	void load(IBehaviorTreeNew* bt, const json& j) override;
};