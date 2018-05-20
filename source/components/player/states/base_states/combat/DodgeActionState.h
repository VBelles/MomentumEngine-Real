#pragma once

#include "components/player/states/GroundedActionState.h"

class DodgeActionState : public GroundedActionState {
private:
	CTimer timer;
	float movingTime = frames2sec(10);
	float recoveryTime = frames2sec(25);
	float invencibilityTime = frames2sec(5);
	float dodgeSpeed = 18.f;
	
	float leavingGroundSpeedMultiplier = 0.25f;

	bool isMoving = true;

	std::string animation = "ataquecaida";
	std::string recoveryAnimation = "jump_caida";

public:
	DodgeActionState(StateManager* stateManager);
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override;
	void onJumpLongButton() override;
	void onStrongAttackButton() override{}
	void onFastAttackButton() override {}
	void onDodgeButton() override {}
	void onReleasePowerButton() override {}
	void onMove(MoveState& moveState) override;
	void onDamage(const TMsgAttackHit& msg) override;

	void onLeavingGround() override;

	void multiplyHorizontalSpeed(float multiplier);
};