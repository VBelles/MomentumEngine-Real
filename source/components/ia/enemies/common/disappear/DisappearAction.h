#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class DisappearAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	std::string animation = "";
	float animationEndPadding = 0.2f;
public:
	DisappearAction();
	DisappearAction(Enemy* enemy, std::string animation);
	int execAction(float delta) override;
	void load(IBehaviorTree* bt, const json& j) override;
	void debugInMenu() override;
};
