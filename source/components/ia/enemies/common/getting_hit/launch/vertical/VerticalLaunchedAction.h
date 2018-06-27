#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"


class VerticalLaunchedAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
public:
	VerticalLaunchedAction(Enemy* enemy);
	int execAction(float delta) override;
};