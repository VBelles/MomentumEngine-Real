#pragma once

#include "components/player/comp_player_model.h"
#include "components/player/states/AirborneActionState.h"

class AirborneWallJumpActionState : public AirborneActionState {

private:
	float backwardsAirDriftFactorLong = 0.4f;
	float sidewaysAirDriftFactorLong = 0.1f;
public:
	AirborneWallJumpActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;

	void SetPose() override { GetRender()->setMesh("data/meshes/pose_long_jump.mesh"); }


};