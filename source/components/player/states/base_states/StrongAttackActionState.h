#pragma once

#include "components/player/comp_player_model.h"
#include "../GroundedActionState.h"


class StrongAttackActionState : public GroundedActionState {
	CTimer timer;
	int warmUpFrames = 20;
	int activeFrames = 5;
	int endingLagFrames = 40;
	float hitboxOutTime;
	float hitEndTime;
	float animationEndTime;

	int IASAFrames = 60;//Interruptible As Soon As
	float interruptibleTime;

	int startLauncherFrames = 20;
	float beginLauncherTime;

	CHandle hitboxHandle;
	int damage = 5;
	AttackPhases phase = AttackPhases::Launch;

public:
	StrongAttackActionState(CHandle playerModelHandle, CHandle hitbox);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void OnJumpHighButton() override {}
	void OnJumpLongButton() override {}
	void OnStrongAttackButton() override {}
	void OnStrongAttackButtonReleased() override;
	void OnFastAttackButton() override {}

	void OnLeavingGround() override;
	void SetPose() override { GetRender()->setMesh("data/meshes/pose_knee.mesh"); }
	void OnHitboxEnter(CHandle entity) override;
};