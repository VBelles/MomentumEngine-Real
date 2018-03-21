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
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void OnJumpHighButton() override;
	void OnJumpLongButton() override;

	virtual void OnLeavingGround();
	void SetFinalRotationAndVelocity();
	void SetPose() override { GetRender()->setMesh("data/meshes/pose_landing.mesh"); }
};