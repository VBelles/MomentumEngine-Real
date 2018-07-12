#pragma once

#include "LandingActionState.h"

class HardLandingActionState : public LandingActionState {
private:
	CTimer hitboxTimer;
	float impactAttackDurationTime = frames2sec(3);
	CTimer fallingAttackHitboxTimer;
	float fallingAttackTime = frames2sec(5);

	CTimer springJumpTimer;
	float springJumpWindowStartTime = frames2sec(15);
	float springJumpWindowEndTime = frames2sec(30);
	bool hasTriedSpringJump = false;

	float stunTime = 2.0f;
	float damage = 0;
	float fallingAttackDamage = 2.f;
	float fallingAttackPowerToGet = 4000.f;

	std::string animation = "landing_hard";

	std::string hitbox = "landing_falling_attack";
	std::string hitboxFallingAttack = "falling_attack";
	std::string hitboxPlummet = "plummet_attack";

public:
	HardLandingActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void setMovementInput(VEC2 input) override {}
	void onJumpHighButton() override;
	void onJumpLongButton() override {}
	void onHitboxEnter(std::string hitbox, CHandle entity) override;
};