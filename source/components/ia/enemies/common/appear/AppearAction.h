#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class AppearAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	std::string animation = "";
	float animationEndPadding = 0.2f;
public:
	AppearAction();
	AppearAction(Enemy* enemy, std::string animation);
	int execAction(float delta) override;
	void load(IBehaviorTree* bt, const json& j) override;
	void debugInMenu() override;
};
