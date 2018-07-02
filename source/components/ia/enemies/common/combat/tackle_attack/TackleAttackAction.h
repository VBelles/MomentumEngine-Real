#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class TackleAttackAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	std::string animation;
	std::string attack;
	float speed;
	float time;
public:
	TackleAttackAction(Enemy* enemy, std::string animation, std::string attack, float speed, float time);
	int execAction(float delta) override;
};