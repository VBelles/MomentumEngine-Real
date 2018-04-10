#pragma once

#include "components/player/comp_player_model.h"
#include "../../GroundedActionState.h"

class JumpSquatActionState : public GroundedActionState {
private:
	CTimer timer;
	int squatFrames = 5;
	float squatTime;

	bool isShortHop = false;

	float enteringVelocity = 0.f;
public:
	JumpSquatActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onFastAttackButton() override {}
	void onStrongAttackButton() override {}
	void onReleasePowerButton() override {}
	void onJumpHighButtonReleased() override;

	void setPose() override { getRender()->setMesh("data/meshes/pose_jump_squat.mesh"); }
	virtual void OnLeavingGround() override;
};