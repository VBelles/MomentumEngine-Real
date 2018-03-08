#pragma once

#include "components/player/comp_player_model.h"
#include "../../GroundedActionState.h"

class JumpSquatActionState : public GroundedActionState {
	CTimer timer;
	int squatFrames = 5;
	float squatTime;

	bool isShortHop = false;

	float enteringVelocity = 0.f;
public:
	JumpSquatActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void OnJumpHighButton() override;
	void OnJumpHighButtonReleased() override;

	void SetPose() override { GetRender()->setMesh("data/meshes/pose_jump_squat.mesh"); }
	virtual void OnLeavingGround() override;
};