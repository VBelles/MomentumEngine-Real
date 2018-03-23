#pragma once

#include "components/player/comp_player_model.h"

class AirborneActionState : public IActionState {
protected:
	CTimer turnAroundTimer;
	bool isTurnAround = false;
	int turnAroundFrames = 3;
	float turnAroundTime;
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
	float backwardsAirDriftFactor = 1.0f;
	float backwardsAirDriftFactorLong = 0.8f;
	PowerStats* enteringPowerStats;

public:
	AirborneActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void OnJumpHighButton() override;
	void OnJumpLongButton() override;
	void OnStrongAttackButton() override;
	void OnFastAttackButton() override;
	void OnReleasePowerButton() override;

	virtual void OnLanding();

};