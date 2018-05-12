#pragma once

#include "components/player/states/GroundedActionState.h"

class VerticalLauncherActionState : public GroundedActionState {
	CTimer timer;

	float hitboxOutTime = frames2sec(5);
	float hitEndTime = frames2sec(8);
	float animationEndTime = frames2sec(17);
	float suspensionTime = 1.5f;

	float interruptibleTime = frames2sec(25);

	float damage = 0;
	AttackPhases phase = AttackPhases::Startup;
	float powerToGet = 2000.f;

	std::string hitbox = "vertical_launcher";

public:
	VerticalLauncherActionState(CHandle playerModelHandle) : GroundedActionState(playerModelHandle, "vertical_launcher"){}
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void setMovementInput(VEC2 input) override;
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onStrongAttackButton() override {}
	void onFastAttackButton() override {}
	void onReleasePowerButton() override {}
	void onLeavingGround() override;
	void onHitboxEnter(std::string hitbox, CHandle entity) override;
};