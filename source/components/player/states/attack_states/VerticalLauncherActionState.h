#pragma once

#include "components/player/comp_player_model.h"
#include "../GroundedActionState.h"


class VerticalLauncherActionState : public GroundedActionState {
	CTimer timer;
	int warmUpFrames = 10;
	int activeFrames = 5;
	int endingLagFrames = 20;
	float hitboxOutTime;
	float hitEndTime;
	float animationEndTime;

	int IASAFrames = 30;//Interruptible As Soon As
	float interruptibleTime;

	CHandle hitboxHandle;
	int damage = 0;

public:
	VerticalLauncherActionState(TCompPlayerModel* player, CHandle hitbox);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void SetMovementInput(VEC2 input) override;
	void OnJumpHighButton() override;
	void OnJumpLongButton() override;

	void OnLeavingGround() override;

	void OnHitboxEnter(CHandle entity) override;
};