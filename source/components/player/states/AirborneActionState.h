#pragma once

#include "components/player/states/IActionState.h"

struct PowerStats;

class AirborneActionState : public IActionState {
protected:
	CTimer2 turnAroundTimer;
	bool isTurnAround = false;
	float turnAroundTime = frames2sec(8);
	float exitYaw;
	float turnAroundRotationSpeed;
	float rotationSpeed = 4.5f;
	VEC3 enterFront;
	float sidewaysdMinAngle = 45.f;
	float backwardsdMinAngle = 125.f;
	float sidewaysMaxDotProduct;
	float backwardsMaxDotProduct;
	float sidewaysAirDriftFactor = 0.7f;
	float sidewaysAirDriftFactorLong = 0.5f;
	float sidewaysAirDriftFactorWall = 0.5f;
	float backwardsAirDriftFactor = 1.0f;
	float backwardsAirDriftFactorLong = 0.8f;
	float backwardsAirDriftFactorWall = 0.9f;
	PowerStats* enteringPowerStats;

	CTimer2 slideWindowTimer;
	float slideWindowTime = frames2sec(5);

	CTimer2 hardLandingTimer;
	float hardLandingTime = 2.2f;

public:
	AirborneActionState(StateManager* stateManager, State state);
	AirborneActionState(StateManager* stateManager, ConcurrentState state);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override;
	void onJumpLongButton() override;
	void onStrongAttackButton() override;
	void onFastAttackButton() override;
	void onReleasePowerButton() override;
	void onDodgeButton() override;
	void onMove(MoveState& moveState) override;
	bool hugWall(const HitState & hitstate);
	void onDamage(const TMsgAttackHit& msg) override;

	virtual void onLanding();
};
