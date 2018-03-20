#pragma once

#include "components/player/comp_player_model.h"
#include "../../AirborneActionState.h"


class PropelLongActionState : public AirborneActionState {
protected:
	CTimer timer;
	int endingFrames = 10;
	float endingTime;
	VEC3 movingVelocity;
	CHandle propelTarget;

public:
	PropelLongActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void OnJumpHighButton() override;
	void OnJumpLongButton() override;

	void OnLanding() override;
	void SetPose() override { GetRender()->setMesh("data/meshes/pose_propel.mesh"); }
};