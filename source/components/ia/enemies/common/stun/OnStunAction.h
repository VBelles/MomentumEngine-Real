#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class OnStunAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	std::string animationStart = "";
	std::string animationLoop = "";
public:
	OnStunAction();
	OnStunAction(Enemy* enemy, std::string animationStart, std::string animationLoop);
	int execAction(float delta) override;
	void load(IBehaviorTree* bt, const json& j) override;
	void debugInMenu() override;
};
