#pragma once

#include "components/player/comp_player_model.h"
#include "../GroundedActionState.h"


class IdleTurnAroundActionState : public GroundedActionState {
	CTimer timer;
	int turnAroundFrames = 12;
	float turnAroundTime;
	float exitYaw;
	float rotationSpeed;

	void RotateToFinalDirection();
public:
	IdleTurnAroundActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void OnJumpHighButton() override;
	void OnJumpLongButton() override;

	virtual void OnLeavingGround();
};