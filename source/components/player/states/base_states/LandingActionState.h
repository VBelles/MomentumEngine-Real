#pragma once

#include "components/player/comp_player_model.h"
#include "../GroundedActionState.h"


class LandingActionState : public GroundedActionState {
protected:
	CTimer timer;
	int landingLagFrames = 9;
	float landingLagTime;
	float enteringSpeed;
public:
	LandingActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override;
	void onJumpLongButton() override;
	void onFastAttackButton() override {}
	void onStrongAttackButton() override {}
	void onReleasePowerButton() override {}

	void SetFinalRotationAndVelocity();
	void setPose() override { getRender()->setMesh("data/meshes/pose_landing.mesh"); }
};