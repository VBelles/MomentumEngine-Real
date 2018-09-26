#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class OnPropelAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	std::string attack = "";
public:
	OnPropelAction();
	OnPropelAction(Enemy* enemy, std::string attack);
	int execAction(float delta) override;
	void load(IBehaviorTree* bt, const json& j) override;
	void debugInMenu() override;
};