#pragma once

#include "components/player/comp_player_model.h"
#include "../GroundedActionState.h"


class TurnAroundActionState : public GroundedActionState {
	CTimer timer;
	int turnAroundFrames = 9;
	float turnAroundTime;
	VEC3 exitVelocityVector;
	float exitYaw;
	float rotationSpeed;
	float velocityLossMultiplier = 0.7f;

	void RotateToFinalDirection();
	void SetFinalVelocity();
public:
	TurnAroundActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void OnJumpHighButton() override;
	void OnJumpLongButton() override;

	virtual void OnLeavingGround();
};