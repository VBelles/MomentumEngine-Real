#pragma once

#include "components/player/comp_player_model.h"
#include "../GroundedActionState.h"

class JumpSquatSpringActionState : public GroundedActionState {
private:
	CTimer timer;
	int squatFrames = 12;
	float squatTime;

public:
	JumpSquatSpringActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void OnJumpHighButton() override {}
	void OnJumpLongButton() override {}
	void OnFastAttackButton() override {}
	void OnStrongAttackButton() override {}
	void OnReleasePowerButton() override {}

	void SetPose() override { GetRender()->setMesh("data/meshes/pose_jump_squat.mesh"); }
	virtual void OnLeavingGround() override;
};