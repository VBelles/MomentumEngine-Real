#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class SuspensionAction : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	std::string soundLoop;
	std::string soundEnd;
	bool soundEmited = false;
	FMOD::Studio::EventInstance* eventInstance = nullptr;
public:
	SuspensionAction();
	SuspensionAction(Enemy* enemy);
	int execAction(float delta) override;
	void load(IBehaviorTree* bt, const json& j) override;
};