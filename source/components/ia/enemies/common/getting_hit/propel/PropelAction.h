#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class PropelAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
public:
	PropelAction(Enemy* enemy);
	int execAction(float delta) override;
};