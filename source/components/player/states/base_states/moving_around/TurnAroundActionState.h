#pragma once

#include "components/player/states/GroundedActionState.h"

class TurnAroundActionState : public GroundedActionState {
private:
	CTimer timer;
	float turnAroundTime = frames2sec(13);
	VEC3 exitVelocityVector;
	float exitYaw;
	float rotationSpeed;
	float velocityLossMultiplier = 0.7f;

	void rotateToFinalDirection();
	void setFinalVelocity();

	std::string animation = "turn_around";

public:
	TurnAroundActionState(StateManager* stateManager);
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override;
	void onJumpLongButton() override;
	void onLeavingGround() override;
};