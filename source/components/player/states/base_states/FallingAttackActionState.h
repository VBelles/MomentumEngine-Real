#pragma once

#include "components/player/comp_player_model.h"
#include "../AirborneActionState.h"


class FallingAttackActionState : public AirborneActionState {
	CTimer timer;
	int warmUpFrames = 12;
	float hitboxOutTime;
	float fallingMultiplier = 2.5f;
	float maxFallingVelocity = 55.f;
	float upwardsVelocity = 18.f;

	CHandle hitboxHandle;
	float damage = 2.f;

	float powerToGet = 4000.f;

public:
	FallingAttackActionState(CHandle playerModelHandle, CHandle hitbox);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void OnJumpHighButton() override {}
	void OnJumpLongButton() override {}
	void OnFastAttackButton() override {}
	void OnStrongAttackButton() override {}
	void OnReleasePowerButton() override {}

	void OnLanding() override;
	void SetPose() override { GetRender()->setMesh("data/meshes/pose_falling_attack.mesh"); }
	void onHitboxEnter(CHandle entity) override;
};