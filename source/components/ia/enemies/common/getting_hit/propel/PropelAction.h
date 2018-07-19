#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class PropelAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	std::string attack;
public:
	PropelAction(Enemy* enemy, std::string attack);
	int execAction(float delta) override;
};