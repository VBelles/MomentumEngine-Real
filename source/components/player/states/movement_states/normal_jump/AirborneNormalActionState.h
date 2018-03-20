#pragma once

#include "components/player/comp_player_model.h"
#include "../../AirborneActionState.h"

class AirborneNormalActionState : public AirborneActionState {
public:
	AirborneNormalActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void OnJumpHighButton() override;
	void OnJumpLongButton() override;

	void SetPose() override { GetRender()->setMesh("data/meshes/pose_jump.mesh"); }

	virtual void OnLanding() override;

};