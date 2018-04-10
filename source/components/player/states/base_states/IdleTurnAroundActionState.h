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
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override;
	void onJumpLongButton() override;

	virtual void onLeavingGround();
};