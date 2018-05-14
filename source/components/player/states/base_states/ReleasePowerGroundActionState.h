#pragma once

#include "components/player/states/GroundedActionState.h"

class ReleasePowerGroundActionState : public GroundedActionState {
private:
	CTimer timer;
	float hitboxOutTime = frames2sec(7);
	float hitEndTime = frames2sec(26);
	float animationEndTime = frames2sec(20);
	int buttonPresses = 1;
	float stunDuration = 3.f;

	float interruptibleTime = frames2sec(25);

	CHandle hitboxSmallHandle;
	CHandle hitboxBigHandle;
	float damage = 0.f;
	AttackPhases phase = AttackPhases::Startup;

	std::string smallHitbox;
	std::string bigHitbox;

public:
	ReleasePowerGroundActionState(CHandle playerModelHandle) : GroundedActionState(playerModelHandle, "liberarenergia"),
		smallHitbox("release_power_small"), bigHitbox("release_power_big") {}
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void setMovementInput(VEC2 input) override {}
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onFastAttackButton() override {}
	void onStrongAttackButton() override {}
	void onReleasePowerButton() override;
	void onHitboxEnter(std::string hitbox, CHandle entity) override;
};