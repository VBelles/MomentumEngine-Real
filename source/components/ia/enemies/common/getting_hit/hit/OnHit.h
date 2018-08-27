#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeAction.h"

class Enemy;

class OnHit : public IBehaviorTreeAction {
private:
	Enemy* enemy = nullptr;
	std::string animation = "";
	std::string particles = "";
	VEC3 particlesOffset;
public:
	OnHit();
	OnHit(Enemy* enemy, std::string animation);
	int execAction(float delta) override;
	void load(IBehaviorTreeNew* bt, const json& j) override;
	void debugInMenu() override;
};