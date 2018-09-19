#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class OnSuspensionAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	std::string animationStart = "";
	std::string animationLoop = "";
public:
	OnSuspensionAction();
	OnSuspensionAction(Enemy* enemy, std::string animationStart, std::string animationLoop);
	int execAction(float delta) override;
	void load(IBehaviorTreeNew* bt, const json& j) override;
	void debugInMenu() override;
};