#pragma once

#include "components/player/comp_player_model.h"
#include "../AirborneActionState.h"


class GrabActionState : public AirborneActionState {
protected:
	CTimer timer;
	int warmUpFrames = 2;
	int activeFrames = 8;
	int endingLagFrames = 15;
	float hitboxOutTime;
	float hitEndTime;
	float animationEndTime;
	float lockDuration = 0.5f;

	int IASAFrames = 25;//Interruptible As Soon As
	float interruptibleTime;

	CHandle hitboxHandle;
	int damage = 0;
	AttackPhases phase = AttackPhases::Startup;

	uint32_t enemyId = getID("enemy");

public:
	GrabActionState(CHandle playerModelHandle, CHandle hitbox);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void SetMovementInput(VEC2 input) override;
	void OnJumpHighButton() override;
	void OnJumpLongButton() override;

	void OnLanding() override;

	void SetPose() override { GetRender()->setMesh("data/meshes/pose_grab.mesh"); }

	void OnHitboxEnter(CHandle entity) override;
};