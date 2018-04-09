#pragma once

#include "components/player/comp_player_model.h"
#include "components/player/states/AirborneActionState.h"

class WallJumpPlummetActionState : public AirborneActionState {
protected:
	CTimer timer;
	float endingTime;
	float deceleration = 0.5f;

	CHandle hitboxHandle;
	int damage = 3;
	float powerToGet = 4000.f;

public:
	WallJumpPlummetActionState(CHandle playerModelHandle, CHandle hitbox);
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
	void OnHitboxEnter(CHandle entity) override;
};