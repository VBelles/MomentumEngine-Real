#pragma once

#include "components/player/comp_player_model.h"
#include "../GroundedActionState.h"


class ReleasePowerGroundActionState : public GroundedActionState {
private:
	CTimer timer;
	int warmUpFrames = 7;
	int activeFrames = 13;
	int endingLagFrames = 10;
	float hitboxOutTime;
	float hitEndTime;
	float animationEndTime;
	int buttonPresses = 1;
	float stunDuration = 3.f;

	int IASAFrames = 25;//Interruptible As Soon As
	float interruptibleTime;

	CHandle hitboxSmallHandle;
	CHandle hitboxBigHandle;
	float damage = 0.f;
	AttackPhases phase = AttackPhases::Startup;

	uint32_t enemyId = getID("enemy");

public:
	ReleasePowerGroundActionState(CHandle playerModelHandle, CHandle hitboxSmall, CHandle hitboxBig);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void SetMovementInput(VEC2 input) override {}
	void OnJumpHighButton() override {}
	void OnJumpLongButton() override {}
	void OnFastAttackButton() override {}
	void OnStrongAttackButton() override {}
	void OnReleasePowerButton() override;

	void SetPose() override { GetRender()->setMesh("data/meshes/pose_grab.mesh"); }

	void OnHitboxEnter(CHandle entity) override;
};