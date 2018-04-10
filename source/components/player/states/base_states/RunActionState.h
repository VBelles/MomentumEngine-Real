#pragma once

#include "components/player/comp_player_model.h"
#include "../GroundedActionState.h"


class RunActionState : public GroundedActionState {
public:
	RunActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	
	void setPose() override { getRender()->setMesh("data/meshes/pose_run.mesh"); }
};