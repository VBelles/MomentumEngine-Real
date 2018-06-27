#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class StunAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
public:
	StunAction(Enemy* enemy);
	int execAction(float delta) override;
};