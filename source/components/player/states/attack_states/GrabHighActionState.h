#pragma once

#include "components/player/comp_player_model.h"
#include "../AirborneActionState.h"


class GrabHighActionState : public AirborneActionState {
	CTimer timer;
	int warmUpFrames = 8;
	int activeFrames = 5;
	int endingLagFrames = 15;
	float hitboxOutTime;
	float hitEndTime;
	float animationEndTime;

	int IASAFrames = 25;//Interruptible As Soon As
	float interruptibleTime;

	CHandle hitboxHandle;
	int damage = 0;

public:
	GrabHighActionState(TCompPlayerModel* player, CHandle hitbox);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void SetMovementInput(VEC2 input) override;
	void OnJumpHighButton() override;
	void OnJumpLongButton() override;

	void OnLanding() override;

	void OnHitboxEnter(CHandle entity) override;
};