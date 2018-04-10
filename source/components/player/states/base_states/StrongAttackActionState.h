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
	float damage = 5.f;
	AttackPhases phase = AttackPhases::Launch;

	float powerToGet = 3000.f;

public:
	StrongAttackActionState(CHandle playerModelHandle, CHandle hitbox);
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onStrongAttackButton() override {}
	void onStrongAttackButtonReleased() override;
	void onFastAttackButton() override {}
	void onReleasePowerButton() override {};

	void onLeavingGround() override;
	void setPose() override { getRender()->setMesh("data/meshes/pose_knee.mesh"); }
	void onHitboxEnter(CHandle entity) override;
};