#pragma once

#include "components/player/comp_player_model.h"
#include "../GroundedActionState.h"


class TurnAroundActionState : public GroundedActionState {
	CTimer timer;
	int turnAroundFrames = 4;
	float turnAroundTime;
	VEC3 exitVelocityVector;
	float rotationSpeed;

	void RotateToFinalDirection();
	void SetFinalVelocity();
public:
	TurnAroundActionState(TCompPlayerModel* player);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void SetMovementInput(VEC2 input) override;
	void OnJumpHighButton() override;
	void OnJumpLongButton() override;

	virtual void OnLeavingGround();
};