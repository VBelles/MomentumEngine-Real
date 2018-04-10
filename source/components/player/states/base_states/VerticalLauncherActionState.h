#pragma once

#include "components/player/comp_player_model.h"
#include "../GroundedActionState.h"


class VerticalLauncherActionState : public GroundedActionState {
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
	VerticalLauncherActionState(CHandle playerModelHandle, CHandle hitbox);
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void setMovementInput(VEC2 input) override;
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onStrongAttackButton() override {}
	void onFastAttackButton() override {}
	void onReleasePowerButton() override {}

	void OnLeavingGround() override;
	void setPose() override { getRender()->setMesh("data/meshes/pose_vertical_launcher.mesh"); }
	void onHitboxEnter(CHandle entity) override;
};