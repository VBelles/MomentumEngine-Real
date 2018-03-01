#pragma once

#include "components/player/comp_player_model.h"
#include "../GroundedActionState.h"


class FastAttackActionState : public GroundedActionState {
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
	bool isLauncher = true;

	CHandle hitboxHandle;
	float damage = 2;

public:
	FastAttackActionState(CHandle playerModelHandle, CHandle hitbox);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void SetMovementInput(VEC2 input) override;
	void OnJumpHighButton() override;
	void OnJumpLongButton() override;
	void OnFastAttackButtonReleased() override;

	void OnLeavingGround() override;

	void OnHitboxEnter(CHandle entity) override;
};