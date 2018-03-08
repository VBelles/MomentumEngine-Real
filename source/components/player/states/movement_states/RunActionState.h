#pragma once

#include "components/player/comp_player_model.h"
#include "../GroundedActionState.h"


class RunActionState : public GroundedActionState {
public:
	RunActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void OnJumpHighButton() override;
	void OnJumpLongButton() override;

	virtual void OnLeavingGround();
	void SetPose() override { GetRender()->setMesh("data/meshes/pose_run.mesh"); }
};