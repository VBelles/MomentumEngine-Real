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

	int IASAFrames = 25;//Interruptible As Soon As
	float interruptibleTime;

	CHandle hitboxHandle;
	float damage = 0;

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