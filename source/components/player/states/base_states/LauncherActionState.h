#pragma once

#include "components/player/states/GroundedActionState.h"

class LauncherActionState : public GroundedActionState {
protected:
	CTimer timer;
	float hitboxOutTime = frames2sec(5);
	float hitEndTime = frames2sec(8);
	float animationEndTime = frames2sec(17);
	float suspensionTime = 1.5f;

	float interruptibleTime = frames2sec(25);

	float damage = 0;
	AttackPhases phase = AttackPhases::Startup;
	float powerToGet = 2000.f;

	std::string animation;
	std::string hitbox;

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