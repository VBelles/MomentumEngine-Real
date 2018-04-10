#pragma once

#include "components/player/comp_player_model.h"
#include "components/player/states/AirborneActionState.h"

class AirborneNormalActionState : public AirborneActionState {
public:
	AirborneNormalActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;

	void setPose() override { getRender()->setMesh("data/meshes/pose_jump.mesh"); }


};