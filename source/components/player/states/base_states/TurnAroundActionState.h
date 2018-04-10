#pragma once

#include "components/player/states/GroundedActionState.h"


class TurnAroundActionState : public GroundedActionState {
	CTimer timer;
	int turnAroundFrames = 9;
	float turnAroundTime;
	VEC3 exitVelocityVector;
	float exitYaw;
	float rotationSpeed;
	float velocityLossMultiplier = 0.7f;

	void rotateToFinalDirection();
	void setFinalVelocity();
public:
	TurnAroundActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override;
	void onJumpLongButton() override;

	virtual void onLeavingGround();
};