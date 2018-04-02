#pragma once

#include "components/player/comp_player_model.h"
#include "LandingActionState.h"


class FallingAttackLandingActionState : public LandingActionState {
	CTimer hitboxTimer;
	int impactAttackDurationFrames = 3;
	float impactAttackDurationTime;
	int newLandingLagFrames = 40;
	float stunTime = 1.5f;

	CHandle hitboxHandle;
	int damage = 0;
public:
	FallingAttackLandingActionState(CHandle playerModelHandle, CHandle hitbox);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void SetMovementInput(VEC2 input) override;
	void OnJumpHighButton() override;
	void OnJumpLongButton() override;

	void OnHitboxEnter(CHandle entity) override;
	void SetPose() override { GetRender()->setMesh("data/meshes/pose_landing.mesh"); }
};