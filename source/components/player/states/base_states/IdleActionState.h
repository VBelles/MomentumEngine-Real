#pragma once

#include "components/player/comp_player_model.h"
#include "../GroundedActionState.h"


class IdleActionState : public GroundedActionState {
public:
	IdleActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;

	void SetPose() override { GetRender()->setMesh("data/meshes/pose_idle.mesh"); }
};