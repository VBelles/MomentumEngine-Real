#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class RangedAttackAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	std::string cancelCondition = "";
	std::string animation = "";
	std::string attack = "";
	bool attackLaunched = false;
public:
	RangedAttackAction();
	RangedAttackAction(Enemy* enemy, std::string animation, std::string attack, std::string cancelCondition = "");
	int execAction(float delta) override;
	void load(IBehaviorTreeNew* bt, const json& j) override;
};