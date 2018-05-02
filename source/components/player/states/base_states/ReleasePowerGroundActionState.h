#pragma once

#include "components/player/states/GroundedActionState.h"

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
	void onHitboxEnter(CHandle entity) override;
};