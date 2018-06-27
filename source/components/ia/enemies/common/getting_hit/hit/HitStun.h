#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class HitStun : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	std::string animation;
public:
	HitStun(Enemy* enemy, std::string animation);
	int execAction(float delta) override;
};