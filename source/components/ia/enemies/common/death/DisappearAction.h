#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class DisappearAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	std::string animation;
public:
	DisappearAction(Enemy* enemy, std::string animation);
	int execAction(float delta) override;
};