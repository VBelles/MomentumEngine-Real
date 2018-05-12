#pragma once

#include "LandingActionState.h"

class FallingAttackLandingActionState : public LandingActionState {

private:
	CTimer hitboxTimer;
	float impactAttackDurationTime = frames2sec(3);
	CTimer fallingAttackHitboxTimer;
	float fallingAttackTime = frames2sec(5);
	float newLandingLagTime = frames2sec(40);
	float stunTime = 1.5f;
	CTimer springJumpTimer;
	float springJumpWindowStartTime = frames2sec(8);
	float springJumpWindowEndTime = frames2sec(28);
	bool hasTriedSpringJump = false;

	float damage = 0;

	std::string hitbox = "landing_falling_attack";
	std::string hitboxFallingAttack = "falling_attack";
	std::string hitboxPlummet = "plummet_attack";

	float fallingAttackDamage = 2.f;
	float fallingAttackPowerToGet = 4000.f;

public:
	FallingAttackLandingActionState(CHandle playerModelHandle) : LandingActionState(playerModelHandle, "hard_landing") {}
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void setMovementInput(VEC2 input) override {}
	void onJumpHighButton() override;
	void onJumpLongButton() override {}
	void onHitboxEnter(std::string hitbox, CHandle entity) override;
};