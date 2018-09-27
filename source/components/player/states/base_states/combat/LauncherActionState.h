#pragma once

#include "components/player/states/GroundedActionState.h"

class LauncherActionState : public GroundedActionState, public AttackState {
protected:
	
	float suspensionTime = 1.5f;

	float damage = 0;
	float stunTime = 2.2f;
	float powerToGet = 2000.f;

	CTimer movementTimer;

	std::string animation;
	std::string constructorHitbox;

public:
	LauncherActionState(StateManager* stateManager, State state, std::string animation, std::string hitbox);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void setMovementInput(VEC2 input) override;
	void onJumpHighButton() override;
	void onJumpLongButton() override;
	void onStrongAttackButton() override {}
	void onFastAttackButton() override {}
	void onDodgeButton() override;
	void onReleasePowerButton() override {}
	void onLeavingGround() override;
};
