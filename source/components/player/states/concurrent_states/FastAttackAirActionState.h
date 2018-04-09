#pragma once

#include "components/player/comp_player_model.h"
#include "../AirborneActionState.h"


class FastAttackAirActionState : public AirborneActionState {
	CTimer timer;
	int warmUpFrames = 5;
	int activeFrames = 8;
	int endingLagFrames = 18;
	float hitboxOutTime;
	float hitEndTime;
	float animationEndTime;

	int IASAFrames = 28;//Interruptible As Soon As
	float interruptibleTime;

	CHandle hitboxHandle;
	int damage = 1;
	AttackPhases phase = AttackPhases::Launch;

	float powerToGet = 1000.f;

public:
	FastAttackAirActionState(CHandle playerModelHandle, CHandle hitbox);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void SetMovementInput(VEC2 input) override {}
	void OnJumpHighButton() override {}
	void OnJumpLongButton() override {}
	void OnFastAttackButton() override {}
	void OnStrongAttackButton() override {}
	void OnReleasePowerButton() override {}
	void OnFastAttackButtonReleased() override {} //posible comportamiento extra

	void SetPose() override { GetRender()->setMesh("data/meshes/pose_punch.mesh"); }
	void OnHitboxEnter(CHandle entity) override;
};