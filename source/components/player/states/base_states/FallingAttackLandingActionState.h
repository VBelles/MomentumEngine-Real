#pragma once

#include "LandingActionState.h"

class FallingAttackLandingActionState : public LandingActionState {
	CTimer hitboxTimer;
	int impactAttackDurationFrames = 3;
	float impactAttackDurationTime;
	int newLandingLagFrames = 40;
	float stunTime = 1.5f;
	CTimer springJumpTimer;
	int springJumpWindowStartFrame = 8;
	float springJumpWindowStartTime;
	int springJumpWindowEndFrame = 28;
	float springJumpWindowEndTime;
	bool hasTriedSpringJump = false;

	CHandle hitboxHandle;
	int damage = 0;

public:
	FallingAttackLandingActionState(CHandle playerModelHandle, CHandle hitbox);
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void setMovementInput(VEC2 input) override {}
	void onJumpHighButton() override;
	void onJumpLongButton() override {}

	void onHitboxEnter(CHandle entity) override;
	void setPose() override;
};