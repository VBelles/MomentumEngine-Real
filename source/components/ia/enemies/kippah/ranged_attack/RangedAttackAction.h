#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Kippah;
class IBehaviorTreeCondition;

class RangedAttackAction : public IBehaviorTreeAction {
private:
	Kippah* enemy = nullptr;
	IBehaviorTreeCondition* cancelCondition = nullptr;
	std::string animation;
	std::string attack;
	bool attackLaunched = false;
public:
	RangedAttackAction(Kippah* enemy, std::string animation, std::string attack, IBehaviorTreeCondition* cancelCondition = nullptr);
	int execAction(float delta) override;
};