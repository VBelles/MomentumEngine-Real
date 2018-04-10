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
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void setMovementInput(VEC2 input) override {}
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onFastAttackButton() override {}
	void onStrongAttackButton() override {}
	void onReleasePowerButton() override {}
	void onFastAttackButtonReleased() override;

	void OnLeavingGround() override;
	void setPose() override { getRender()->setMesh("data/meshes/pose_punch.mesh"); }
	void onHitboxEnter(CHandle entity) override;
};