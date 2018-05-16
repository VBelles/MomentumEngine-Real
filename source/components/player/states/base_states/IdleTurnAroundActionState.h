#pragma once

#include "components/player/states/GroundedActionState.h"

class IdleTurnAroundActionState : public GroundedActionState {
private:
	CTimer timer;
	float turnAroundTime = frames2sec(12);
	float exitYaw;
	float rotationSpeed;

	void rotateToFinalDirection();

public:
	IdleTurnAroundActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override;
	void onJumpLongButton() override;
	void onLeavingGround() override;
};