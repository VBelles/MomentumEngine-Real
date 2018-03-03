#pragma once

#include "components/player/comp_player_model.h"
#include "../GroundedActionState.h"


class LandingActionState : public GroundedActionState {
	CTimer timer;
	int landingLagFrames = 5;
	float landingLagTime;
	float enteringSpeed;
public:
	LandingActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void SetMovementInput(VEC2 input) override;
	void OnJumpHighButton() override;
	void OnJumpLongButton() override;
	virtual void OnLeavingGround();
	void SetFinalRotationAndVelocity();
};