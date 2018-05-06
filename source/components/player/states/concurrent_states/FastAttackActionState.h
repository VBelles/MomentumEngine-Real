#pragma once

#include "components/player/states/GroundedActionState.h"

class Hitboxes;

class FastAttackActionState : public GroundedActionState {
private:
	CTimer timer;
	int warmUpFrames = 10;
	int activeFrames = 15;
	int endingLagFrames = 5;
	float hitboxOutTime;
	float hitEndTime;
	float animationEndTime;

	int IASAFrames = 13;//Interruptible As Soon As
	float interruptibleTime;

	int startLauncherFrames = 15;
	float beginLauncherTime;

	CHandle hitboxHandle;
	float damage = 1.f;
	AttackPhases phase = AttackPhases::Launch;

	float powerToGet = 2000.f;

	std::string hitbox;


public:
	FastAttackActionState(CHandle playerModelHandle) 
		: GroundedActionState(playerModelHandle, "melee"), hitbox("fast_attack") {}
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void setMovementInput(VEC2 input) override {}
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onFastAttackButton() override {}
	void onStrongAttackButton() override {}
	void onReleasePowerButton() override {}
	void onFastAttackButtonReleased() override;
	void onLeavingGround() override;
	void onHitboxEnter(std::string hitbox, CHandle entity) override;

};