#pragma once

#include "components/player/states/GroundedActionState.h"

class HorizontalLauncherActionState : public GroundedActionState {
	CTimer timer;
	int warmUpFrames = 5;
	int activeFrames = 8;
	int endingLagFrames = 17;
	float hitboxOutTime;
	float hitEndTime;
	float animationEndTime;
	float suspensionTime = 1.5f;

	int IASAFrames = 25;//Interruptible As Soon As
	float interruptibleTime;

	CHandle hitboxHandle;
	float damage = 0;
	AttackPhases phase = AttackPhases::Startup;

	float powerToGet = 2000.f;

	std::string hitbox = "horizontal_launcher";

public:
	HorizontalLauncherActionState(CHandle playerModelHandle) : GroundedActionState(playerModelHandle, "vertical_launcher") {}
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onStrongAttackButton() override {}
	void onFastAttackButton() override {}
	void onReleasePowerButton() override {}
	void onLeavingGround() override;
	void onHitboxEnter(CHandle entity) override;
};