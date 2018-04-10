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
	float damage = 1.f;
	AttackPhases phase = AttackPhases::Launch;

	float powerToGet = 1000.f;

public:
	FastAttackAirActionState(CHandle playerModelHandle, CHandle hitbox);
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void setMovementInput(VEC2 input) override {}
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onFastAttackButton() override {}
	void onStrongAttackButton() override {}
	void onReleasePowerButton() override {}
	void onFastAttackButtonReleased() override {} //posible comportamiento extra

	void setPose() override { getRender()->setMesh("data/meshes/pose_punch.mesh"); }
	void onHitboxEnter(CHandle entity) override;
};