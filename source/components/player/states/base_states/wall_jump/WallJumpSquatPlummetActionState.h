#pragma once

#include "components/player/comp_player_model.h"
#include "components/player/states/AirborneActionState.h"

class WallJumpSquatPlummetActionState : public AirborneActionState {
protected:
	CTimer timer;
	int endingFrames = 20;
	float endingTime;
	float frontVelocity = 30.f;
	float verticalVelocity = -30.f;

public:
	WallJumpSquatPlummetActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onFastAttackButton() override {}
	void onStrongAttackButton() override {}
	void onReleasePowerButton() override {}

	void setPose() override { getRender()->setMesh("data/meshes/pose_propel.mesh"); }
};