#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"


class OnHorizontalLaunchAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	std::string animation;
public:
	OnHorizontalLaunchAction(Enemy* enemy, std::string animation);
	int execAction(float delta) override;
};