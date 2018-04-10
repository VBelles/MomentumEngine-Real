#pragma once

#include "components/player/comp_player_model.h"

class AirborneActionState : public IActionState {
protected:
	CTimer turnAroundTimer;
	bool isTurnAround = false;
	int turnAroundFrames = 8;
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
	float sidewaysAirDriftFactorWall = 0.5f;
	float backwardsAirDriftFactor = 1.0f;
	float backwardsAirDriftFactorLong = 0.8f;
	float backwardsAirDriftFactorWall = 0.9f;
	PowerStats* enteringPowerStats;



public:
	AirborneActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override;
	void onJumpLongButton() override;
	void onStrongAttackButton() override;
	void onFastAttackButton() override;
	void onReleasePowerButton() override;

	virtual void onLanding();
	virtual void onShapeHit(const PxControllerShapeHit &hit) override;

};