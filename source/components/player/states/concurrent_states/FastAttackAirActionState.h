#pragma once

#include "components/player/states/AirborneActionState.h"

class FastAttackAirActionState : public AirborneActionState {
	CTimer timer;
	int warmUpFrames = 5;
	int activeFrames = 8;
	int endingLagFrames = 18;
	float hitboxOutTime;
	float hitEndTime;
	float animationEndTime;

	int IASAFrames = 28;//Interruptible As Soon As
	float interruptibleTime;

	float damage = 1.f;
	AttackPhases phase = AttackPhases::Launch;

	float powerToGet = 1000.f;

	std::string hitbox;

public:
	FastAttackAirActionState(CHandle playerModelHandle)
		: AirborneActionState(playerModelHandle, "melee"), hitbox("fast_attack") {}
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void setMovementInput(VEC2 input) override {}
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onFastAttackButton() override {}
	void onStrongAttackButton() override {}
	void onReleasePowerButton() override {}
	void onFastAttackButtonReleased() override {} //posible comportamiento extra

	void onHitboxEnter(std::string hitbox, CHandle entity) override;
};