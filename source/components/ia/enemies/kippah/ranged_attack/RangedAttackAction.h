#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Kippah;

class RangedAttackAction : public IBehaviorTreeAction {
private:
	Kippah* enemy = nullptr;
	std::string animation;
	std::string attack;
	bool attackLaunched = false;
public:
	RangedAttackAction(Kippah* enemy, std::string animation, std::string attack);
	int execAction(float delta) override;
};