#pragma once

#include "components/player/comp_player_model.h"
#include "../GroundedActionState.h"


class FastAttackActionState : public GroundedActionState {
	CTimer timer;
	int warmUpFrames = 5;
	int activeFrames = 5;
	int endingLagFrames = 10;
	float hitboxOutTime;
	float hitEndTime;
	float animationEndTime;

	int IASAFrames = 13;//Interruptible As Soon As
	float interruptibleTime;

	int startLauncherFrames = 15;
	float beginLauncherTime;

	CHandle hitboxHandle;
	float damage = 1.f;
	AttackPhases phase = AttackPhases::Launch;

	float powerToGet = 2000.f;

public:
	FastAttackActionState(CHandle playerModelHandle, CHandle hitbox);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void SetMovementInput(VEC2 input) override {}
	void OnJumpHighButton() override {}
	void OnJumpLongButton() override {}
	void OnFastAttackButton() override {}
	void OnStrongAttackButton() override {}
	void OnReleasePowerButton() override {}
	void OnFastAttackButtonReleased() override;

	void OnLeavingGround() override;
	void SetPose() override { GetRender()->setMesh("data/meshes/pose_punch.mesh"); }
	void OnHitboxEnter(CHandle entity) override;
};