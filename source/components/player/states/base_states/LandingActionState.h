#pragma once

#include "components/player/states/GroundedActionState.h"

class LandingActionState : public GroundedActionState {
protected:
	CTimer timer;
	float landingLagTime = frames2sec(9);
	float enteringSpeed;

	std::string animation = "landing_soft";

public:
	LandingActionState(StateManager* stateManager, State state = Landing);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override;
	void onJumpLongButton() override;
	void onFastAttackButton() override {}
	void onStrongAttackButton() override {}
	void onReleasePowerButton() override {}

	void SetFinalRotationAndVelocity();
	
};