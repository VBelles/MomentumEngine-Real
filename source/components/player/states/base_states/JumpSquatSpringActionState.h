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
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onFastAttackButton() override {}
	void onStrongAttackButton() override {}
	void onReleasePowerButton() override {}

	void setPose() override { getRender()->setMesh("data/meshes/pose_jump_squat.mesh"); }
	virtual void OnLeavingGround() override;
};