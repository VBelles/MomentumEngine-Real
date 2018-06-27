#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"


class HorizontalLaunchedAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
public:
	HorizontalLaunchedAction(Enemy* enemy);
	int execAction(float delta) override;
};