#pragma once

#include "components/player/comp_player_model.h"
#include "../GroundedActionState.h"


class HorizontalLauncherActionState : public GroundedActionState {
	CTimer timer;
	int warmUpFrames = 5;
	int activeFrames = 8;
	int endingLagFrames = 17;
	float hitboxOutTime;
	float hitEndTime;
	float animationEndTime;
	float suspensionTime = 1.5f;

	int IASAFrames = 25;//Interruptible As Soon As
	float interruptibleTime;

	CHandle hitboxHandle;
	int damage = 0;
	AttackPhases phase = AttackPhases::Startup;

	float powerToGet = 2000.f;

public:
	HorizontalLauncherActionState(CHandle playerModelHandle, CHandle hitbox);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void OnJumpHighButton() override {}
	void OnJumpLongButton() override {}
	void OnStrongAttackButton() override {}
	void OnFastAttackButton() override {}
	void OnReleasePowerButton() override {}

	void OnLeavingGround() override;
	void SetPose() override { GetRender()->setMesh("data/meshes/pose_horizontal_launcher.mesh"); }
	void OnHitboxEnter(CHandle entity) override;
};