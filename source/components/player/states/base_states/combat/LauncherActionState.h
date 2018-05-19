#pragma once

#include "components/player/states/GroundedActionState.h"
#include "components/player/states/AttackState.h"

class LauncherActionState : public GroundedActionState, public AttackState {
protected:
	
	float suspensionTime = 1.5f;

	float damage = 0;
	float powerToGet = 2000.f;

	std::string animation;
	std::string constructorHitbox;

public:
	LauncherActionState(StateManager* stateManager, State state, std::string animation, std::string hitbox);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onStrongAttackButton() override {}
	void onFastAttackButton() override {}
	void onReleasePowerButton() override {}
	void onLeavingGround() override;
};