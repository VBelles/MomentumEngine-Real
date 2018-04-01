#pragma once

#include "components/player/comp_player_model.h"
#include "components/player/states/AirborneActionState.h"

class WallJumpPlummetActionState : public AirborneActionState {
protected:
	CTimer timer;
	int endingFrames = 30;
	float endingTime;
	float frontVelocity = 20.f;
	float verticalVelocity = -20.f;
	float deceleration = 0.1f;

public:
	WallJumpPlummetActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void OnJumpHighButton() override;
	void OnJumpLongButton() override;
	void OnFastAttackButton() override;
	void OnStrongAttackButton() override;
	//void OnReleasePowerButton() override {}

	void SetPose() override { GetRender()->setMesh("data/meshes/pose_long_jump.mesh"); }
	virtual void OnLanding() override;
};