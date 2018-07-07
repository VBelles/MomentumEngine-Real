#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class OnBlockAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	std::string animationStart;
	std::string animationLoop;
public:
	OnBlockAction(Enemy* enemy, std::string animationStart, std::string animationLoop);
	int execAction(float delta) override;
};