#pragma once

#include "components/player/comp_player_model.h"
#include "../../AirborneActionState.h"

class AirborneLongActionState : public AirborneActionState {
public:
	AirborneLongActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void SetMovementInput(VEC2 input) override;
	void OnJumpHighButton() override;
	void OnJumpLongButton() override;

	void SetPose() override { GetRender()->setMesh("data/meshes/pose_long_jump.mesh"); }
	virtual void OnLanding() override;

};