#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class OnGrabAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
public:
	OnGrabAction();
	OnGrabAction(Enemy* enemy);
	int execAction(float delta) override;
	void load(IBehaviorTree* bt, const json& j) override;
};
