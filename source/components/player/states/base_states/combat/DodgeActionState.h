#pragma once

#include "components/player/states/GroundedActionState.h"

class DodgeActionState : public GroundedActionState {
private:
	CTimer timer;
	float movingTime = frames2sec(12);
	float recoveryTime = frames2sec(15);
	float regularRecoveryTime = recoveryTime;
	float staleRecoveryTime = frames2sec(70);
	CTimer invencibilityTimer;
	float invencibilityTime = frames2sec(9);
	float takeActionTime = frames2sec(3);
	float dodgeSpeed = 22.f;
	
	static constexpr int numberOfDodgesToStale = 4;
	CTimer staleTimers[numberOfDodgesToStale - 1];//si abusas, te comes el lag
	float staleTime = 2.5f;
	int staleTimerIndex = 0;
	int currentNumberOfCloseDodges = 0;
	bool isInterruptible = true;

	float leavingGroundSpeedMultiplier = 0.25f;

	bool isMoving = true;

	std::string animation = "dodge";
	std::string recoveryAnimation = "landing_hard";

public:
	DodgeActionState(StateManager* stateManager);
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override;
	void onJumpLongButton() override;
	void onStrongAttackButton() override;
	void onFastAttackButton() override;
	void onDodgeButton() override;
	void onReleasePowerButton() override;
	void onMove(MoveState& moveState) override;
	void onDamage(const TMsgAttackHit& msg) override;

	void onLeavingGround() override;

	void multiplyHorizontalSpeed(float multiplier);
};
