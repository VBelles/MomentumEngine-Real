#pragma once

#include "components/player/comp_player_model.h"
#include "../../AirborneActionState.h"

class AirborneLongActionState : public AirborneActionState {
public:
	AirborneLongActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;

	void setPose() override { getRender()->setMesh("data/meshes/pose_long_jump.mesh"); }
};