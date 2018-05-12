#pragma once

#include "components/player/states/GroundedActionState.h"

class StrongAttackActionState : public GroundedActionState {
private:
	CTimer timer;
	float hitboxOutTime = frames2sec(60);
	float hitEndTime = frames2sec(10);
	float animationEndTime = frames2sec(60);

	int IASAFrames = 60;//Interruptible As Soon As
	float interruptibleTime = frames2sec(60);

	float beginLauncherTime = frames2sec(20);

	float damage = 5.f;
	AttackPhases phase = AttackPhases::Launch;

	float powerToGet = 3000.f;

	std::string hitbox = "strong_attack";

public:
	StrongAttackActionState(CHandle playerModelHandle) : GroundedActionState(playerModelHandle, "melee_strong") {}
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onStrongAttackButton() override {}
	void onStrongAttackButtonReleased() override;
	void onFastAttackButton() override {}
	void onReleasePowerButton() override {};
	void onLeavingGround() override;
	void onHitboxEnter(std::string hitbox, CHandle entity) override;
};