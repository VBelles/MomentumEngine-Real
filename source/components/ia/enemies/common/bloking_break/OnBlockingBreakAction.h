#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class OnBlockingBreakAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	std::string animation;
public:
	OnBlockingBreakAction(Enemy* enemy, std::string animation);
	int execAction(float delta) override;
};