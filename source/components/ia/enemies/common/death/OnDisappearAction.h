#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class OnDisappearAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	std::string animation;
public:
	OnDisappearAction(Enemy* enemy, std::string animation);
	int execAction(float delta) override;
};