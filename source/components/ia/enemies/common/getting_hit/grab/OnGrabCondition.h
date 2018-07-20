#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeCondition.h"

class Enemy;

class OnGrabCondition : public IBehaviorTreeCondition {
private:
	Enemy* enemy = nullptr;
public:
	OnGrabCondition();
	OnGrabCondition(Enemy* enemy);
	bool testCondition(float delta) override;
	void load(IBehaviorTreeNew* bt, const json& j) override;
};