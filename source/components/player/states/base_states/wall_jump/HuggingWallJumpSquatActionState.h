#pragma once

#include "components/player/comp_player_model.h"
#include "components/player/states/AirborneActionState.h"

class HuggingWallJumpSquatActionState : public AirborneActionState {
protected:
	CTimer timer;
	int endingFrames = 4;
	float endingTime;

public:
	HuggingWallJumpSquatActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void OnJumpHighButton() override {}
	void OnJumpLongButton() override {}
	void OnFastAttackButton() override {}
	void OnStrongAttackButton() override {}
	void OnReleasePowerButton() override {}

	void SetPose() override { GetRender()->setMesh("data/meshes/pose_propel.mesh"); }
};