#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"


class OnAirborneAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	std::string animation;
public:
	OnAirborneAction(Enemy* enemy, std::string animation);
	int execAction(float delta) override;
};