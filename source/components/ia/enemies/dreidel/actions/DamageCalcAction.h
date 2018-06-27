#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class DamageCalcAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
public:
	DamageCalcAction(Enemy* enemy);
	int execAction(float delta) override;
};