#pragma once

#include "components/player/comp_player_model.h"
#include "../AirborneActionState.h"


class FallingAttackActionState : public AirborneActionState {
	CTimer timer;
	int warmUpFrames = 20;
	float hitboxOutTime;
	float fallingAcceleration = -70.f;
	float maxFallingVelocity = 45.f;

	CHandle hitboxHandle;
	float damage = 3;

public:
	FallingAttackActionState(CHandle playerHandle, CHandle hitbox);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void SetMovementInput(VEC2 input) override;
	void OnJumpHighButton() override;
	void OnJumpLongButton() override;

	void OnLanding() override;

	void OnHitboxEnter(CHandle entity) override;
};